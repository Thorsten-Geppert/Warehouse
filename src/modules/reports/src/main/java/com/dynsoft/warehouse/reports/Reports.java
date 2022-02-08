package com.dynsoft.warehouse.reports;

import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.HashMap;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

import net.sf.jasperreports.engine.JasperPrint;
import net.sf.jasperreports.engine.JasperPrintManager;
import net.sf.jasperreports.engine.JasperFillManager;
import net.sf.jasperreports.engine.JRException;
import net.sf.jasperreports.engine.export.JRPdfExporter;
import net.sf.jasperreports.engine.JRExporterParameter;
import net.sf.jasperreports.engine.JRExporter;
import net.sf.jasperreports.view.JasperViewer;

import org.apache.commons.lang3.SystemUtils;

import com.dynsoft.warehouse.reports.lib.ConfigurationType;
import com.dynsoft.warehouse.reports.lib.ConfigurationLibrary;

public class Reports {

	private static String [] availableOptions = {
		"show",
		"print",
		"printdirect",
		"pdf"
	};

    public static void main(String[] args) {
		if(args.length != 5) {
			if(args.length == 1 && args[0].equals("--help"))
				usage();
			else
				printError("ERROR_ARGUMENTS");
			return;
		}

		try {
			Class.forName("org.postgresql.Driver");
		} catch(Exception exception) {
			printError("ERROR_JDBC_LOAD");
			return;
		}

		final String optionConfigurationFile = args[0];
		final String optionCompany           = args[1];
		final String optionReportName        = args[2];
		final String optionTodo              = args[4];
		int optionDataId                     = 0;

		try {
			optionDataId = Integer.parseInt(args[3]);
		} catch(Exception exception) {
			printError("ERROR_DATA_ID");
			return;
		}

		if(!Arrays.asList(availableOptions).contains(optionTodo)) {
			printError("ERROR_TODO");
			return;
		}

		Connection connection = null;

		ConfigurationLibrary configurationLibrary = new ConfigurationLibrary();
		ConfigurationType configurationType = configurationLibrary.readConfiguration(
			optionConfigurationFile, false
		);
		if(configurationType != null) {
			if(configurationType.isOk()) {
				try {
					connection = DriverManager.getConnection(
						 String.format(
							"jdbc:postgresql://%s:%d/%s",
							configurationType.getHostname(),
							configurationType.getPort(),
							configurationType.getDatabase()
						),
						configurationType.getUsername(),
						configurationType.getPassword()
					);
				} catch(Exception exception) {
					printError("ERROR_DATABASE_CONNECTION");
					connection = null;
				}
			} else {
				printError("ERROR_CONFIGURATION_OK");
			}
		} else {
			printError("ERROR_CONFIGURATION_LOAD");
		}

		if(connection != null) {
			boolean found          = false;
			String companyHostname = null;
			String companyDatabase = null;
			String companyUsername = null;
			String companyPassword = null;
			int companyPort        = 5432;

			try {
				PreparedStatement statement = connection.prepareStatement("SELECT * FROM enterprises WHERE company = ?");
				statement.setString(1, optionCompany);
				statement.execute();
				ResultSet resultSet = statement.getResultSet();
				if(resultSet.next()) {
					found = true;
					companyHostname = resultSet.getString("hostname");
					companyDatabase = resultSet.getString("database");
					companyUsername = resultSet.getString("username");
					companyPassword = resultSet.getString("password");
					companyPort     = resultSet.getInt("port");
				}
				resultSet.close();
				statement.close();
			} catch(SQLException exception) {
			}

			try {
				connection.close();
			} catch(Exception exception) {
			}

			if(found) {
				try {
					connection = DriverManager.getConnection(
						 String.format(
							"jdbc:postgresql://%s:%d/%s",
							companyHostname,
							companyPort,
							companyDatabase
						),
						companyUsername,
						companyPassword
					);
				} catch(Exception exception) {
					printError("ERROR_COMPANY_CONNECTION");
					connection = null;
				}
				
				if(connection != null) {
					boolean reportUse = false;
					String reportPath = null;
					try {
						Statement statement = connection.createStatement();
						statement.execute("SELECT * FROM common");
						ResultSet resultSet = statement.getResultSet();
						if(resultSet.next()) {
							reportUse = resultSet.getBoolean("reportUse");

							String separator = "/";
							if(SystemUtils.IS_OS_WINDOWS) {
								reportPath = resultSet.getString("reportPathWindows");
								separator = "\\";
							} else if(SystemUtils.IS_OS_MAC) {
								reportPath = resultSet.getString("reportPathMacOS");
							} else {
								reportPath = resultSet.getString("reportPathUnix");
							}

							if(reportPath.length() > 0 && !reportPath.substring(reportPath.length() - 1).equals(separator))
								reportPath += separator + optionReportName + ".jasper";
						}
						resultSet.close();
						statement.close();
					} catch(SQLException exception) {
					}
					
					if(!reportUse) {
						printError("ERROR_NO_REPORT_USE");
					} else {
						String table = null;
						try {
							PreparedStatement statement = connection.prepareStatement("SELECT name FROM jasper_reports WHERE report = ?");
							statement.setString(1, optionReportName);
							statement.execute();
							ResultSet resultSet = statement.getResultSet();
							if(resultSet.next()) {
								table = resultSet.getString("name");
							}
							resultSet.close();
							statement.close();
						} catch(SQLException exception) {
						}

						if(table != null) {
							String key = null;
							if(table.equals("orders"))
								key = "orderId";
							else if(table.equals("clients"))
								key = "clientId";
							else if(table.equals("products"))
								key = "productId";
							else if(table.equals("vouchers"))
								key = "voucherId";
							else if(table.equals("purchases"))
								key = "purchaseId";

							int count = 0;
							try {
								PreparedStatement statement = connection.prepareStatement("SELECT COUNT(*) AS counter FROM \"" + table + "\" WHERE \"" + key + "\" = ?");
								statement.setInt(1, optionDataId);
								statement.execute();
								ResultSet resultSet = statement.getResultSet();
								if(resultSet.next())
									count = resultSet.getInt("counter");
								resultSet.close();
								statement.close();
							} catch(SQLException exception) {
							}

							if(count > 0) {
								File file = new File(reportPath);
								if(file.exists() && file.isFile()) {
									try {
										HashMap parametersHashMap = new HashMap();
										parametersHashMap.put("id", optionDataId);

										JasperPrint jasperPrint = JasperFillManager.fillReport(reportPath, parametersHashMap, connection);

										if(optionTodo.equals("show")) {
											JasperViewer jasperViewer = new JasperViewer(jasperPrint, false);
											jasperViewer.setVisible(true);
										} else if(optionTodo.equals("pdf")) {
											JFileChooser pdfFileChooser = new JFileChooser();
											pdfFileChooser.setFileFilter(new FileFilter() {
												@Override
												public boolean accept(File f) {
													return true;
												}

												@Override
												public String getDescription() {
													return "PDF";
												}
											});
											pdfFileChooser.setSelectedFile(new File("export.pdf"));
											if(pdfFileChooser.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
												JRExporter exporter = new JRPdfExporter();
												exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME, pdfFileChooser.getSelectedFile().getAbsolutePath());
												exporter.setParameter(JRExporterParameter.JASPER_PRINT, jasperPrint);
												exporter.exportReport();
											}
										} else if(optionTodo.equals("print")) {
											JasperPrintManager.printReport(jasperPrint, true);
										} else if(optionTodo.equals("printdirect")) {
											JasperPrintManager.printReport(jasperPrint, false);
										}
										
										printError("DONE");
									} catch(JRException exception) {
										printError("ERROR_REPORT_GENERATION");
									}
								} else {
									printError("ERROR_REPORT_FILE_DOES_NOT_EXIST");
								}
							} else {
								printError("ERROR_DATA_NOT_FOUND");
							}
						} else {
							printError("ERROR_REPORT_IN_TABLE_NOT_FOUND");
						}
					}

					try {
						connection.close();
					} catch(Exception exception) {
					}
				}
			} else {
				printError("ERROR_COMPANY_NOT_FOUND");
			}
		}
    }

	private static void usage() {
		System.out.println(
			"Name: " + Configuration.NAME + "\n" +
			"Version: " + Configuration.VERSION + "\n" +
			"Usage: java -jar program.jar " +
			" <Warehouse configuration file.ini>" +
			" <Company>" +
			" <Report name>" +
			" <Data id>" +
			" <Todo (show|print|printdirect|pdf)>"
		);
	}

	private static void printError(String error) {
		System.out.println(error);
	}

}

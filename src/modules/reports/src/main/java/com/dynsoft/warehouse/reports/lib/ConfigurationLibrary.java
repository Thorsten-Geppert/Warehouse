package com.dynsoft.warehouse.reports.lib;

import java.io.IOException;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;

public class ConfigurationLibrary {

	public ConfigurationLibrary() {
	}

	public ConfigurationType readConfiguration(String filename, boolean dontNullConfigurationType) {
		int ok = 0;
		final int okMustHave = 5;

		ConfigurationType configurationType = new ConfigurationType();

		try {
			FileInputStream configFileInputStream = new FileInputStream(filename);
			BufferedReader configBufferedReader = new BufferedReader(new InputStreamReader(configFileInputStream));

			String line;
			while((line = configBufferedReader.readLine()) != null) {
				int position = line.indexOf('=');
				if(position >= 0) {
					String key = line.substring(0, position);
					String value = line.substring(position + 1);

					if(!value.isEmpty()) {
						if(key.equals("hostname")) {
							configurationType.setHostname(value);
							ok++;
						} else if(key.equals("database")) {
							configurationType.setDatabase(value);
							ok++;
						} else if(key.equals("username")) {
							configurationType.setUsername(value);
							ok++;
						} else if(key.equals("password")) {
							configurationType.setPassword(value);
							ok++;
						} else if(key.equals("port")) {
							try {
								configurationType.setPort(Integer.parseInt(value));
							} catch(Exception exception) {
							}

							ok++; // Entweder aus Config, sonst 5432
						}
					}
				}
			}

			configBufferedReader.close();
			configFileInputStream.close();

			configurationType.setOk(ok == okMustHave);
		} catch(Exception exception) {
			if(!dontNullConfigurationType)
				configurationType = null;
		}

		return configurationType;
	}

}

package com.dynsoft.warehouse.reports.lib;

public class ConfigurationType {

	private boolean ok;
	private String hostname;
	private String database;
	private String username;
	private String password;
	private int port;

	public ConfigurationType() {
		setOk(false);
		setPort(5432);
	}

	public void setHostname(String hostname) {
		this.hostname = hostname;
	}

	public String getHostname() {
		return hostname;
	}

	public void setDatabase(String database) {
		this.database = database;
	}

	public String getDatabase() {
		return database;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getUsername() {
		return username;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getPassword() {
		return password;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public int getPort() {
		return port;
	}

	public void setOk(boolean ok) {
		this.ok = ok;
	}

	public boolean isOk() {
		return ok;
	}

	@Override
	public String toString() {
		return String.format(
			"      Ok: %s\n" +
			"Hostname: %s\n" +
			"Database: %s\n" +
			"Username: %s\n" +
			"Password: %s\n" +
			"    Port: %d",
			isOk() ? "yes" : "no",
			getHostname(),
			getDatabase(),
			getUsername(),
			getPassword(),
			getPort()
		);
	}

}

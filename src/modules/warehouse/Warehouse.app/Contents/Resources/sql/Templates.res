Templates.Exists
	SELECT COUNT(*) AS counter FROM ":table:" WHERE name = :name: AND name != :oldName:

KeyValueDatabaseLibrary.Get
	SELECT value FROM ":table:" WHERE key = :key:

KeyValueDatabaseLibrary.ExistsByKey
	SELECT COUNT(*) AS counter FROM ":table:" WHERE key = :key:

KeyValueDatabaseLibrary.ExistsByKeyAndValue
	SELECT COUNT(*) AS counter FROM ":table:" WHERE key = :key: AND value = :value:

KeyValueDatabaseLibrary.GetAll
	SELECT * FROM ":table:" ORDER BY key, value

KeyValueDatabaseLibrary.Insert
	INSERT INTO ":table:" VALUES (:key:, :value:)

KeyValueDatabaseLibrary.Update
	UPDATE ":table:" SET value = :value: WHERE key = :key:

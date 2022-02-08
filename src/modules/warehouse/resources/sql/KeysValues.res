KeysValues.Insert
	INSERT INTO ":table:" (id, key, value) VALUES (:id:, :key:, :value:)

KeysValues.Remove
	DELETE FROM ":table:" WHERE id = :id:

KeysValues.Select
	SELECT * FROM ":table:" WHERE id = :id: ORDER BY key, value

Reports.GetContent
	SELECT content FROM reports WHERE name = :name:

Reports.RemoveAll
	DELETE FROM reports

Reports.Save
	INSERT INTO reports (name, content) VALUES (:name:, :content:)

Reports.Get
	SELECT content FROM reports WHERE name = :name:

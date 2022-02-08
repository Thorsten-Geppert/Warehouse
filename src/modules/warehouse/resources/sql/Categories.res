Categories.ExistsInCategory
	SELECT COUNT(*) AS counter FROM ":table:" WHERE "parentId" = :parentId: AND name = :name:

Categories.ExistsInRoot
	SELECT COUNT(*) AS counter FROM ":table:" WHERE "parentId" IS NULL AND name = :name:

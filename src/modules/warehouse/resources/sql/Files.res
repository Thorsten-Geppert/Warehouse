Files.GetByCategory
	SELECT * FROM files WHERE "categoryId" = :categoryId: ORDER BY LOWER(:orderBy:)

Files.GetWithoutCategory
	SELECT * FROM files WHERE "categoryId" IS NULL ORDER BY LOWER(:orderBy:)

Files.Move
	UPDATE files SET "categoryId" = :categoryId: WHERE "fileId" = :fileId:

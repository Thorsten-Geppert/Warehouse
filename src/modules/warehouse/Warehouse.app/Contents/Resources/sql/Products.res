Products.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		CAST("productId" AS TEXT) ILIKE :productId:
	AND
		COALESCE("articleNumber", '') ILIKE :articleNumber:
	AND
		COALESCE("shortName", '') ILIKE :shortName:
	AND
		(
			COALESCE("name1", '') ILIKE :name1:
		OR
			COALESCE("name2", '') ILIKE :name2:
		OR
			COALESCE("name3", '') ILIKE :name3:
		OR
			COALESCE("name4", '') ILIKE :name4:
		)
	AND
		COALESCE("description", '') ILIKE :description:
	AND
		COALESCE("note", '') ILIKE :note:
	AND
		COALESCE("isbn10", '') ILIKE :isbn10:
	AND
		COALESCE("isbn13", '') ILIKE :isbn13:
	AND
		COALESCE("ean", '') ILIKE :ean:
	AND
		COALESCE("group", '') ILIKE :group:
	AND
		COALESCE(CAST("distributorId" AS TEXT), '') ILIKE :distributorId:
	AND
		COALESCE("manufacturer", '') ILIKE :manufacturer:
	AND
		COALESCE("publisher", '') ILIKE :publisher:
	ORDER BY
		"shortName", name1

Products.FindAvailable
	SELECT
		*
	FROM
		":table:"
	WHERE
		CAST("productId" AS TEXT) ILIKE :productId:
	AND
		COALESCE("articleNumber", '') ILIKE :articleNumber:
	AND
		COALESCE("shortName", '') ILIKE :shortName:
	AND
		(
			COALESCE("name1", '') ILIKE :name1:
		OR
			COALESCE("name2", '') ILIKE :name2:
		OR
			COALESCE("name3", '') ILIKE :name3:
		OR
			COALESCE("name4", '') ILIKE :name4:
		)
	AND
		COALESCE("description", '') ILIKE :description:
	AND
		COALESCE("note", '') ILIKE :note:
	AND
		COALESCE("isbn10", '') ILIKE :isbn10:
	AND
		COALESCE("isbn13", '') ILIKE :isbn13:
	AND
		COALESCE("ean", '') ILIKE :ean:
	AND
		COALESCE("group", '') ILIKE :group:
	AND
		COALESCE(CAST("distributorId" AS TEXT), '') ILIKE :distributorId:
	AND
		COALESCE("manufacturer", '') ILIKE :manufacturer:
	AND
		COALESCE("publisher", '') ILIKE :publisher:
	AND
		available = TRUE
	ORDER BY
		"shortName", name1

Products.ReferenceByClientId
	SELECT
		DISTINCT products."productId" AS id
	FROM
		products
	WHERE
		products."distributorId" = :distributorId:
	ORDER BY
		products."productId"

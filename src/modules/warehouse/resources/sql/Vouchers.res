Vouchers.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		CAST("voucherId" AS TEXT) ILIKE :voucherId:
	AND
		COALESCE("description", '') ILIKE :description:
	AND
		COALESCE("invoiceId", '') ILIKE :invoiceId:
	AND
		COALESCE("company", '') ILIKE :company:
	AND
		COALESCE("notes", '') ILIKE :notes:
	AND (
			CAST(extract('year' FROM "invoiceDate") AS TEXT) LIKE :year:
		OR
			CAST(extract('year' FROM "createdAt") AS TEXT) LIKE :year:
	)
	ORDER BY
		"invoiceDate" DESC, company, "invoiceId"

Vouchers.Statistics
	SELECT
		*
	FROM
		vouchers
	WHERE
		vouchers."invoiceDate" BETWEEN :from: AND :to:

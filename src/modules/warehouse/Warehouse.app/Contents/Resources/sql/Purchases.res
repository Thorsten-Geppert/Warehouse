Purchases.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		CAST("purchaseId" AS TEXT) ILIKE :purchaseId:
	AND
		CAST("distributorId" AS TEXT) ILIKE :distributorId:
	AND
		COALESCE(CAST("orderId" AS TEXT), '') ILIKE :orderId:
	AND
		COALESCE(CAST("distributorAddress" AS TEXT), '') ILIKE :distributorAddress:
	AND
		COALESCE(CAST("billingAddress" AS TEXT), '') ILIKE :billingAddress:
	AND
		COALESCE(CAST("deliveryAddress" AS TEXT), '') ILIKE :deliveryAddress:
	AND
		COALESCE(CAST("purchaser" AS TEXT), '') ILIKE :purchaser:
	AND
		COALESCE(CAST("informations" AS TEXT), '') ILIKE :informations:
	AND (
			CAST(extract('year' FROM "done") AS TEXT) LIKE :year:
		OR
			CAST(extract('year' FROM "createdAt") AS TEXT) LIKE :year:
	)
	ORDER BY
		"purchaseId" DESC, "distributorId"

Purchases.GetDone
	SELECT done FROM purchases WHERE "purchaseId" = :purchaseId:

Purchases.Done
	UPDATE purchases SET done = :doneTime: WHERE "purchaseId" = :purchaseId:

Purchases.NotDone
	UPDATE purchases SET done = NULL WHERE "purchaseId" = :purchaseId:

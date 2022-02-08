Orders.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		CAST("orderId" AS TEXT) ILIKE :orderId:
	AND
		CAST("clientId" AS TEXT) ILIKE :clientId:
	AND
		COALESCE(CAST("invoiceId" AS TEXT), '') ILIKE ':invoiceId:'
	AND
		COALESCE(CAST("kind" AS TEXT), '') ILIKE ':kind:'
	AND
		(COALESCE("billAddressSalutation", '') ILIKE :billAddressSalutation: OR COALESCE("shippingAddressSalutation", '') ILIKE :shippingAddressSalutation:)
	AND
		(COALESCE("billAddressTitle", '') ILIKE :billAddressTitle: OR COALESCE("shippingAddressTitle", '') ILIKE :shippingAddressTitle:)
	AND
		(COALESCE("billAddressFirstname", '') ILIKE :billAddressFirstname: OR COALESCE("shippingAddressFirstname", '') ILIKE :shippingAddressFirstname:)
	AND
		(COALESCE("billAddressAddress", '') ILIKE :billAddressAddress: OR COALESCE("shippingAddressAddress", '') ILIKE :shippingAddressAddress:)
	AND
		(COALESCE("billAddressZipcode", '') ILIKE :billAddressZipcode: OR COALESCE("shippingAddressZipcode", '') ILIKE :shippingAddressZipcode:)
	AND
		(COALESCE("billAddressCity", '') ILIKE :billAddressCity: OR COALESCE("shippingAddressCity", '') ILIKE :shippingAddressCity:)
	AND
		(COALESCE("billAddressLand", '') ILIKE :billAddressLand: OR COALESCE("shippingAddressLand", '') ILIKE :shippingAddressLand:)
	AND
		(COALESCE("billAddressName1", '') ILIKE :billAddressName1: OR COALESCE("billAddressName2", '') ILIKE :billAddressName2: OR COALESCE("billAddressName3", '') ILIKE :billAddressName3: OR COALESCE("billAddressName4", '') ILIKE :billAddressName4: OR COALESCE("shippingAddressName1", '') ILIKE :shippingAddressName1: OR COALESCE("shippingAddressName2", '') ILIKE :shippingAddressName2: OR COALESCE("shippingAddressName3", '') ILIKE :shippingAddressName3: OR COALESCE("shippingAddressName4", '') ILIKE :shippingAddressName4:)
	AND (
			CAST(extract('year' FROM "invoiceDate") AS TEXT) LIKE :year:
		OR
			CAST(extract('year' FROM "payed") AS TEXT) LIKE :year:
		OR
			CAST(extract('year' FROM "createdAt") AS TEXT) LIKE :year:
		OR
			CAST(extract('year' FROM "when") AS TEXT) LIKE :year:
	)
	:onlyUnpayed:
	ORDER BY
		"orderId" DESC, "clientId"

Orders.OnlyUnpayed
	AND "payed" IS NULL AND kind = 1

Orders.Invoice
	UPDATE ":table:" SET "invoiceId" = :invoiceId:, "invoiceDate" = :invoiceDate: WHERE "orderId" = :orderId:

Orders.OrderInvoiceId
	SELECT NEXTVAL('"orders_orderInvoiceId_seq"') AS "invoiceId"

Orders.CreditInvoiceId
	SELECT NEXTVAL('"orders_creditInvoiceId_seq"') AS "invoiceId"

Orders.Now
	SELECT CURRENT_DATE AS "invoiceDate" FROM ":table:" LIMIT 1

Orders.Statistics
	SELECT
		orders."invoiceId" AS "invoiceId",
		orders."invoiceDate" AS "invoiceDate",
		orders."orderId" AS "orderId",
		clients."clientId" AS "clientId",
		clients."ownClientId" AS "ownClientId",
		orders_positions."productId" AS "productId",
		orders_positions."articleNumber" AS "articleNumber",
		orders_positions."beforeTax" AS "beforeTax",
		orders_positions.tax AS tax,
		orders_positions."afterTax" AS "afterTax",
		orders_positions."taxLevel" AS "taxLevel",
		orders_positions."orderId",
		orders."orderId"
	FROM
		orders_positions
	INNER JOIN
		orders ON orders."orderId" = orders_positions."orderId"
	INNER JOIN
		clients ON clients."clientId" = orders."clientId"
	WHERE
		orders."invoiceId" IS NOT NULL AND orders."invoiceDate" IS NOT NULL
	AND
		orders."invoiceDate" BETWEEN :from: AND :to:
	:product:
	:client:

	UNION

	SELECT
		orders."invoiceId" AS "invoiceId",
		orders."invoiceDate" AS "invoiceDate",
		orders."orderId" AS "orderId",
		clients."clientId" AS "clientId",
		clients."ownClientId" AS "ownClientId",
		orders_positions."productId" AS "productId",
		orders_positions."articleNumber" AS "articleNumber",
		orders_positions."beforeTax" AS "beforeTax",
		orders_positions.tax AS tax,
		orders_positions."afterTax" AS "afterTax",
		orders_positions."taxLevel" AS "taxLevel",
		orders_positions."orderId",
		orders."orderId"
	FROM
		orders
	INNER JOIN
		orders AS linked_orders ON linked_orders."linkedOrderId" = orders."orderId"
	INNER JOIN
		orders_positions ON orders_positions."orderId" = linked_orders."orderId"
	INNER JOIN
		clients ON clients."clientId" = orders."clientId"
	AND
		orders."invoiceDate" BETWEEN :from: AND :to:
	:product:
	:client:
			
Orders.StatisticsProductId
	AND orders_positions."productId" = :productId:

Orders.StatisticsClientId
	AND clients."clientId" = :clientId:

Orders.ReferenceByProductId
	SELECT
		DISTINCT orders."orderId" AS id
	FROM
		orders
	INNER JOIN
		orders_positions ON orders_positions."orderId" = orders."orderId"
	WHERE
		orders_positions."productId" = :productId:
	ORDER BY
		orders."orderId"

Orders.ReferenceByClientId
	SELECT
		orders."orderId" AS id
	FROM
		orders
	WHERE
		orders."clientId" = :clientId:
	ORDER BY
		orders."orderId"

Orders.ReferenceByOrderId
	SELECT
		orders."orderId" AS id
	FROM
		orders
	WHERE
		orders."basedOn" = :orderId:
	ORDER BY
		orders."orderId"

Orders.IncrementPrints
	UPDATE orders SET prints = prints + 1 WHERE "orderId" = :orderId:

Orders.GetPrints
	SELECT prints FROM orders WHERE "orderId" = :orderId:

Orders.GetPayed
	SELECT payed FROM orders WHERE "orderId" = :orderId:

Orders.Payed
	UPDATE orders SET payed = :payedTime: WHERE "orderId" = :orderId:

Orders.NotPayed
	UPDATE orders SET payed = NULL WHERE "orderId" = :orderId:

Orders.Link
	UPDATE orders SET "linkedOrderId" = :linkedOrderId:, "linkedOrderIndex" = :linkedOrderIndex: WHERE "orderId" = :orderId:

Orders.Unlink
	UPDATE orders SET "linkedOrderId" = NULL, "linkedOrderIndex" = NULL WHERE "linkedOrderId" = :orderId:

Orders.IsLinked
	SELECT "orderId" FROM orders WHERE "linkedOrderId" = :orderId:

Orders.SetMonition
	UPDATE orders SET "monitionState" = :monitionState: WHERE "orderId" = :orderId:

Orders.GetBookedStockAndQuantityAndProduct
	SELECT
		orders_positions.stock AS stock,
		orders_positions."productId" AS "productId",
		orders_positions.amount AS quantity
	FROM
		orders_positions
	INNER JOIN
		orders ON orders."orderId" = orders_positions."orderId"
	WHERE
		(orders."linkedOrderId" = :linkedOrderId: OR orders."orderId" = :orderId:)
	AND
		orders_positions."bookedOut" = FALSE
	AND
		orders_positions.stock IS NOT NULL

Orders.BookOut
	UPDATE
		orders_positions
	SET
		"bookedOut" = TRUE
	WHERE
		"orderId" = (
			SELECT
				"orderId"
			FROM
				"orders"
			WHERE
				"linkedOrderId" = :linkedOrderId:
		)
	OR
		"orderId" = :orderId:

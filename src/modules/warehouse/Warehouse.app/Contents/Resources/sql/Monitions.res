Monitions.GetUnpayedOrdersStart
	SELECT
		"orderId",
		"invoiceId",
		"invoiceDate",
		"clientId",
		"afterTax" AS price,
		"monitionState"
	FROM
		orders
	WHERE
		CAST("clientId" AS TEXT) ILIKE :clientId:
	AND
		CAST("orderId" AS TEXT) ILIKE :orderId:
	AND
		CAST("invoiceId" AS TEXT) ILIKE :invoiceId:
	AND
		("invoiceDate" BETWEEN :invoiceDateFrom: AND :invoiceDateTo:)
	AND
		payed IS NULL
	AND
		monition = TRUE
	AND
		kind = 1
	AND
		(

Monitions.GetUnpayedOrdersState
	"monitionState" = {s:monitionState}

Monitions.GetUnpayedOrdersStop
	)
	ORDER BY
		"monitionState", "invoiceDate"

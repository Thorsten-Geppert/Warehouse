StockBook.GetByStockAndProduct
	SELECT id FROM ":table:" WHERE "stock" = :stock: AND "product" = :product:

StockBook.GetQuantityByStockAndProduct
	SELECT SUM(quantity) AS quantity FROM ":table:" WHERE stock = :stock: AND product = :product:

StockBook.GetReservedByStockAndProduct
	SELECT SUM(amount) AS quantity FROM "orders_positions" WHERE stock = :stock: AND "productId" = :product: AND "bookedOut" = FALSE

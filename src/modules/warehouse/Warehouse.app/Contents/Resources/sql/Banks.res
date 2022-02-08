Banks.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		COALESCE("bank", '') ILIKE '%:bank:%'
	AND
		COALESCE("bankCode", '') ILIKE '%:bankCode:%'
	AND
		COALESCE("bic", '') ILIKE '%:bic:%'
	ORDER BY
		bank, "bankCode"

Banks.DeleteAll
	DELETE FROM ":table:"

Banks.Exists
	SELECT COUNT(*) AS counter FROM :table: WHERE bank = :bank: AND "bankCode" = :bankCode: AND bic = :bic:

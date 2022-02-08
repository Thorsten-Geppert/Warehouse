Clients.Find
	SELECT
		*
	FROM
		":table:"
	WHERE
		(CAST("clientId" AS TEXT) ILIKE :clientId: OR CAST("ownClientId" AS TEXT) ILIKE :ownClientId:)
	AND
		COALESCE("salutation", '') ILIKE :salutation:
	AND
		COALESCE("title", '') ILIKE :title:
	AND
		COALESCE("firstname", '') ILIKE :firstname:
	AND
		COALESCE("address", '') ILIKE :address:
	AND
		COALESCE("zipcode", '') ILIKE :zipcode:
	AND
		COALESCE("city", '') ILIKE :city:
	AND
		COALESCE("land", '') ILIKE :land:
	AND
		(COALESCE("email1", '') ILIKE :email1: OR COALESCE("email2", '') ILIKE :email2:)
	AND
		(COALESCE("name1", '') ILIKE :name1: OR COALESCE("name2", '') ILIKE :name2: OR COALESCE("name3", '') ILIKE :name3: OR COALESCE("name4", '') ILIKE :name4:)
	ORDER BY
		name1, firstname, name2, name3, name4

Clients.FindActive
	SELECT
		*
	FROM
		":table:"
	WHERE
		(CAST("clientId" AS TEXT) ILIKE :clientId: OR CAST("ownClientId" AS TEXT) ILIKE :ownClientId:)
	AND
		COALESCE("salutation", '') ILIKE :salutation:
	AND
		COALESCE("title", '') ILIKE :title:
	AND
		COALESCE("firstname", '') ILIKE :firstname:
	AND
		COALESCE("address", '') ILIKE :address:
	AND
		COALESCE("zipcode", '') ILIKE :zipcode:
	AND
		COALESCE("city", '') ILIKE :city:
	AND
		COALESCE("land", '') ILIKE :land:
	AND
		(COALESCE("email1", '') ILIKE :email1: OR COALESCE("email2", '') ILIKE :email2:)
	AND
		(COALESCE("name1", '') ILIKE :name1: OR COALESCE("name2", '') ILIKE :name2: OR COALESCE("name3", '') ILIKE :name3: OR COALESCE("name4", '') ILIKE :name4:)
	AND
		active = TRUE
	ORDER BY
		name1, firstname, name2, name3, name4

Clients.ExistsOwnClientId
	SELECT COUNT(*) AS counter FROM ":table:" WHERE "ownClientId" = :ownClientId:

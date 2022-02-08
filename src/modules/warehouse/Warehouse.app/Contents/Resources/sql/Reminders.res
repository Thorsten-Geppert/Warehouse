Reminders.GetAll
	SELECT
		*
	FROM
		"reminders"
	ORDER BY
		"dateTime"

Reminders.GetByUsername
	SELECT
		*
	FROM
		"reminders"
	WHERE
		"username" = :username:
	ORDER BY
		"dateTime"

Reminders.GetByModuleAndUsername
	SELECT
		*
	FROM
		"reminders"
	WHERE
		"username" = :username:
	AND
		":field:" = :id:
	ORDER BY
		"dateTime"

Reminders.Insert
	INSERT INTO "reminders" (
		"clientId",
		"productId",
		"orderId",
		"voucherId",
		"purchaseId",
		"username",
		"dateTime",
		"notes"
	) VALUES (
		:clientId:,
		:productId:,
		:orderId:,
		:voucherId:,
		:purchaseId:,
		:username:,
		:dateTime:,
		:notes:
	)

Reminders.Update
	UPDATE "reminders" SET
		"clientId" = :clientId:,
		"productId" = :productId:,
		"orderId" = :orderId:,
		"voucherId" = :voucherId:,
		"purchaseId" = :purchaseId:,
		"username" = :username:,
		"dateTime" = :dateTime:,
		"notes" = :notes:
	WHERE
		"reminderId" = :reminderId:

Reminders.Delete
	DELETE FROM "reminders" WHERE "reminderId" = :reminderId:

Reminders.Sequence
	SELECT CURRVAL('"reminders_reminderId_seq"') AS "referenceId"

Reminders.ServerTime
	SELECT NOW() AS "serverTime"

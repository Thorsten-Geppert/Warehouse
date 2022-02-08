Users.SetOpenToRemind
	UPDATE ":table:" SET "openToRemind" = :openToRemind: WHERE username = :username:

Users.GetOpenToRemind
	SELECT "openToRemind" FROM ":table:" WHERE username = :username:

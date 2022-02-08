<html>
<head>
</head>
<body>

	<br>
	<br>
	<br>
	<br>
	<table border="0" width="100%">
		<tr>
			<td colspan="3">
				<font size="1">{s:company} - {s:executive} - {s:address1} - {s:zipcode} {s:city}</font>
				<br>
			</td>
		</tr>
		<tr>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:shippingAddress}
				</font>
			</td>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:translatedShippingAddress}
				</font>
			</td>
			<td valign="top" align="right" width="34%">
				<font size="1">
					<br>&nbsp;<br>
					Tel.: {s:telephone}<br>
					Fax: {s:fax}<br>
					Mail: {s:email}<br>
					Web: {s:web}<br>
					USt-IdNr: {s:salesTaxIdentificationNumber}<br>
					Steuernummer: {s:taxNumber}<br>
				</font>
				<br>
				<font size="1">
					Kundennummer: {s:clientId}<br>
					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>
					Steuernummer: {s:orderTaxNumber}<br>
					Rechnungsnummer: {s:invoiceId}<br>
					Rechnungsdatum: {s:invoiceDate}<br>
					Datum: {s:when}<br>
					Versand: {s:delivery}<br>
					Zeichen: {s:orderNumber}<br>
					Eigentümer: {s:owner}<br>
				</font>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<font size="6"><b>{s:kind}: {s:orderId} {s:copy}</b></font>
			</td>
		</tr>
	</table>

	<br>
	<br>

	<table border="1" width="100%">
		<tr>
			<td width="4%" align="right"><font size="1"><b>Nr.</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Menge</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Preis</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Gesamt</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Rabatt</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Summe</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Steuer</b></font></td>
			<td width="20%" align="right"><font size="1"><b>Betrag</b></font></td>
		</tr>
	</table>

</body>
</html>

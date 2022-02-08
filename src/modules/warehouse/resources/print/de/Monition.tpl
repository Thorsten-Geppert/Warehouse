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
					{s:billAddress}
				</font>
			</td>
			<td valign="top" width="33%">
				<font size="1">
					<br>&nbsp;<br>
					{s:translatedBillAddress}
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
					USt-IdNr: {s:orderSalesTaxIdentificationNumber}<br>
					Steuernummer: {s:orderTaxNumber}<br>
					Rechnungsnummer: {s:invoiceId}<br>
					Rechnungsdatum: {s:invoiceDate}<br>
					Versand: {s:delivery}<br>
					Zeichen: {s:orderNumber}<br>
				</font>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<font size="6"><b>Zahlungsaufforderung - Stufe {s:monitionState}</b></font>
			</td>
		</tr>
	</table>

	<br>
	<br>

	<p>{s:prependText}</p>

	<br>
	<br>

	<table border="1" width="100%">
		<tr>
			<td width="10%" align="right"><font size="1"><b>Stufe</b></font></td>
			<td width="20%" align="left"><font size="1"><b>Datum</b></font></td>
			<td width="25%" align="right"><font size="1"><b>Rechnungsnummer</b></font></td>
			<td width="25%" align="right"><font size="1"><b>Auftragsnummer</b></font></td>
			<td width="20%" align="right"><font size="1"><b>Betrag</b></font></td>
		</tr>
		<tr>
			<td width="10%" align="right"><font size="1">{s:monitionState}</font></td>
			<td width="20%" align="left"><font size="1">{s:invoiceDate}</font></td>
			<td width="25%" align="right"><font size="1">{s:invoiceId}</font></td>
			<td width="25%" align="right"><font size="1">{s:orderId}</font></td>
			<td width="20%" align="right"><font size="1">{s:price}</font></td>
		</tr>
		<tr>
			<td width="80%" align="right" colspan="4"><font size="1">Mahngebühren:</font></td>
			<td width="20%" align="right"><font size="1">{s:monitionPrice}</font></td>
		</tr>
		<tr>
			<td width="80%" align="right" colspan="4"><font size="1"><b>Gesamt:</b></font></td>
			<td width="20%" align="right"><font size="1"><b>{s:sum}</b></font></td>
		</tr>
	</table>

	<p>{s:appendText}</p>

	<div style="page-break-before:always">

</body>
</html>

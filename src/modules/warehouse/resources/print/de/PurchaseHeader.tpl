<html>
<head>
</head>

<body>
	<table cellspacing="0" cellpadding="0">
		<tr>
			<td><b>Rechnungsadresse</b></td>
			<td>&nbsp;&nbsp;&nbsp;</td>
			<td><b>Lieferadresse</b></td>
		</tr>
		<tr>
			<td><font size="1">{s:billingAddress}</font></td>
			<td>&nbsp;&nbsp;&nbsp;</td>
			<td><font size="1">{s:deliveryAddress}</font></td>
		</tr>
	</table>

	<p>&nbsp;</p><p>&nbsp;</p>

	<p>
		<font size="1">{s:distributorAddress}</font>
	</p>

	<p>&nbsp;</p>

	<p>
		<font size="1">Kundennummer: {s:clientId}</font>
	</p>

	<p>&nbsp;</p>

	<table width="100%" cellspacing="0" cellpadding="0">
		<tr>
			<td><font size="4"><b>Bestellung:</b> {s:generatedId}</td>
			<td align="right">{s:orderDate}</td>
		</tr>
	</table>

	<p>&nbsp;</p><p>&nbsp;</p>

	<table border="1" width="100%">
		<tr>
			<td width="4%" align="right"><font size="1"><b>Nr.</b></font></td>
			<td width="10%" align="right"><font size="1"><b>Menge</b></font></td>
			<td width="20%"><font size="1"><b>Art.-Nr.</b></font></td>
			<td width="38%"><font size="1"><b>Artikel</b></font></td>
			<td width="13%" align="right"><font size="1"><b>Preis</b></font></td>
			<td width="15%" align="right"><font size="1"><b>Gesamt</b></font></td>
		</tr>
	</table>

</body>
</html>

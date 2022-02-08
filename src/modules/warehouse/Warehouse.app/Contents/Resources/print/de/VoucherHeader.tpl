<html>
	<head>
	</head>
	<body>
		<h1>
			{s:description} {s:invoiceId} - {s:invoiceDate}
		</h1>

		<br>
		<br>

		<table border="0">
			<tr>
				<td align="right"><b>Belegnummer:</b></td>
				<td align="right">{s:voucherId}</td>
			</tr>
			<tr>
				<td align="right"><b>Preis:</b></td>
				<td align="right">{s:priceWithoutTax}</td>
			</tr>
			<tr>
				<td align="right"><b>Verminderte Steuer:</b></td>
				<td align="right">{s:reducedTax}</td>
			</tr>
			<tr>
				<td align="right"><b>Volle Steuer:</b></td>
				<td align="right">{s:fullTax}</td>
			</tr>
			<tr>
				<td align="right"><b>Gesamtpreis:</b></td>
				<td align="right"><u>{s:priceWithTax}</u></td>
			</tr>
		</table>

		<br>
		<br>
		<br>

		<table border="0">
			<tr>
				<td colspan="2" width="50%" valign="top">{s:company}</td>
				<td colspan="2" width="50%" valign="top">{s:notes}</td>
			</tr>
		</table>
	</body>
</html>

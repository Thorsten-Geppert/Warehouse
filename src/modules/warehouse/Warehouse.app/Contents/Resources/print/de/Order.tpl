<table border="0" width="100%">
	<tr>
		<td colspan="8"><font size="1">{s:prepandText}</font></td>
	</tr>
	
	{d:positions}
	{d:linked}

	<tr>
		<td colspan="7" align="right"><font size="1">Summe:</font></td>
		<td align="right"><font size="1">{s:sumBeforeTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1">MwSt. {s:fullTaxPercent}%:</font></td>
		<td align="right"><font size="1">{s:sumFullTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1">MwSt. {s:reducedTaxPercent}%:</font></td>
		<td align="right"><font size="1">{s:sumReducedTax} EUR</font></td>
	</tr>
	<tr>
		<td colspan="7" align="right"><font size="1"><b>Betrag:</b></font></td>
		<td align="right"><font size="1"><b><u>{s:sumAfterTax} EUR</u></b></font></td>
	</tr>

	<tr>
		<td colspan="8">
			<font size="1">
				{s:condition}<br><br>
				Bezahlung per {s:payment} {s:bankData}<br><br>
				Gesamtmenge: {s:fullAmount}<br>
				Gesamtgewicht: {s:fullWeight} kg
			</font>
		</td>
	</tr>

	<tr>
		<td colspan="8"><font size="1">{s:appendText}</font></td>
	</tr>
</table>

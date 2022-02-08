<!DOCTYPE html>
<html lang="de">

<head>
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<title>GuG-IT Warehouse - Rechnungen schreiben und fakturieren.</title>
	<meta name="author" content="GuG-IT GbR - Olena und Thorsten Geppert" />
	<meta name="description" content="Erstellen Sie mit dieser Rechnungssoftware Angebote und Aufträge, Rechnungen und Lieferscheine, Mahnungen und Gutschriften. Pflegen Sie Kunden und Produkte.">

	<link rel="stylesheet" type="text/css" href="toolkits/bugs/ie10-viewport-bug-workaround.css">
	<link rel="stylesheet" type="text/css" href="toolkits/bootstrap/css/bootstrap.min.css">
	<link rel="stylesheet" type="text/css" href="styles/styles.css">

	<script src="toolkits/jquery/jquery.min.js"></script>
	<script src="toolkits/bootstrap/js/bootstrap.min.js"></script>
	<script src="/__statistics/statistics.js"></script>

	<link rel="shortcut icon" href="images/Favicon.ico" type="image/x-icon">
	<link rel="icon" href="images/Favicon.ico" type="image/x-icon">
</head>

<body data-spy="scroll" data-target=".navbar" data-offset="50">
	<nav class="navbar navbar-inverse navbar-fixed-top">
		<div class="container-fluid">
			<div class="navbar-header">
				<button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#navigationbar">
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>                        
				</button>
				<div>
					<img src="images/Icon.png" style="height: 35px; margin-top: 7px;" alt="Warehouse-Icon">
				</div>
			</div>
			<div>
				<div class="collapse navbar-collapse" id="navigationbar">
					<ul class="nav navbar-nav navbar-right navigation">
						<li><a href="#home" title="Home"><span class="glyphicon glyphicon-home" aria-hidden="true"></span></a></li>
						<li><a href="#warehouse" title="Warehouse">Warehouse</a></li>
						<li><a href="#functions" title="Funktionen im Überblick">Funktionen</a></li>
						<li><a href="#prices" title="Download, Demoversion, Preise und kaufen">Demoversion</a></li>
						<li><a href="#videos" title="Videos der Installation auf Windows, Linux und FreeBSD">Videos</a></li>
						<li><a href="#more" title="Weitere Informationen zu Warehouse">Informationen</a></li>
						<li><a href="#news" title="Neuigkeiten">News</a></li>
						<li><a href="#macos" title="macOS Beta">macOS Beta</a></li>
						<li><a href="#imprint" title="Impressum, AGB und Datenschutz">Impressum</a></li>
					</ul>
				</div>
			</div>
		</div>
	</nav>    

	<div id="home" class="container-fluid section">
		<div class="slogan">Zeit sparen und mehr Geld verdienen</div>
	</div>

	<div id="warehouse" class="container">
		<div class="row">
			<div class="col-xs-12">
				<h1>Top-Lösung</h1> 
				<p class="title_description_1">für kleine und mittelständige Unternehmen</p>
			</div>
		</div>
		<div class="row">
			<div class="col-xs-12 col-md-6">
				<div class="video">
					<iframe src="https://www.youtube.com/embed/KyzcoS2-WXg" frameborder="0" allowfullscreen class="video"></iframe>
				</div>
			</div>
			<div class="col-xs-12 col-md-6">
				<p>
					Warehouse ist eine Rechnungssoftware. Sie wurde entwickelt, um den Büroarbeitsalltag
					für kleine Unternehmen, Selbständige, Freiberufler
					sowie Handwerker zu entlasten. Arbeiten Sie mit dieser
					Warenwirtschaft flexibel, ob auf Windows, macOS-, Linux- oder
					FreeBSD-Systemen.
				</p>
				<a class="btn_warehouse" href="https://www.gug-it.de/seiten/Warehouse/Kaufen.html" role="button" target="buy" title="Warehouse kaufen">
					<span class="glyphicon glyphicon-thumbs-up" aria-hidden="true"></span>
					<span class="btn_title">Herunterladen und profitieren</span>
				</a>
			</div>
		</div>

		<div class="row">
			<div class="col-xs-12 distance_top">
				<h2>Alle Funktionen</h2> 
				<p class="title_description_2">im schnellen Überblick</p>
			</div>
		</div>
		<div class="row">
			<div class="col-md-4 text-center">
				Kundenverwaltung<br>
				Produktverwaltung<br>
				Lagerverwaltung<br>
				Angebote und Aufträge erstellen<br>
				Rechnungen und Lieferscheine<br>
				Belege und Eingangsrechnungen<br>
			</div>
			<div class="col-md-4 text-center">
				Einkauf<br>
				Wiedervorlagen und Erinnerungen<br>
				Dateien verwalten<br>
				Statistiken<br>
				Notizen erstellen<br>
				Datenbankbasierte Verwaltung<br>
			</div>
			<div class="col-md-4 text-center distance_bottom">
				Multiplattformfähig<br>
				Mehrbenutzerfähig<br>
				Mahnungen<br>
				Textvorlagen<br>
				Export<br>
				uvm.
			</div>
		</div>
	</div>

	<div id="functions" class="container-fluid functions-carousel">
		<div id="carousel-example-generic" class="carousel slide" data-ride="carousel">
			<div class="carousel-inner" role="listbox">
				{s:functions}
			</div>

			<a class="left carousel-control" href="#carousel-example-generic" role="button" data-slide="prev">
				<span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
				<span class="sr-only">Previous</span>
			</a>
			<a class="right carousel-control" href="#carousel-example-generic" role="button" data-slide="next">
				<span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
				<span class="sr-only">Next</span>
			</a>
		</div>
	</div>
	<div id="prices" class="container section">
		<div class="row">
			<div class="col-md-4">
				<div class="center">
					<p class="sale_title">Demoversion</p>
					<p>
						<a href="https://www.gug-it.de/seiten/Warehouse/Kostenlos%20herunterladen.html" target="download" title="Demoversion herunterladen">
							<span class="glyphicon glyphicon-download prices" aria-hidden="true"></span>
						</a>
					</p>
					<p><a href="https://www.gug-it.de/seiten/Warehouse/Kostenlos%20herunterladen.html" target="download" title="Demoversion herunterladen" style="color: #000;">Kostenlose 90-Tage-Testversion</a></p>
				</div>
			</div>
			<div class="col-md-4">
				<div class="center">
					<p class="sale_title">Preise</p>
					<p><a href="https://www.gug-it.de/seiten/Warehouse/Preise.html" target="prices" title="Preise für Warehouse"><span class="glyphicon glyphicon-barcode prices" aria-hidden="true"></span></a></p>
					<p><a href="https://www.gug-it.de/seiten/Warehouse/Preise.html" target="prices" title="Preise für Warehouse" style="color: #000;">119,- Euro<br>(zzgl. 19% MwSt. 141,61 Euro)</a></p>
				</div>
			</div>
			<div class="col-md-4">
				<div class="center">
					<p class="sale_title">Kaufen</p>
					<p><a href="https://www.gug-it.de/seiten/Warehouse/Kaufen.html" target="buy" title="Warehouse kaufen"><span class="glyphicon glyphicon-shopping-cart prices" aria-hidden="true"></span></a></p>
					<p><a href="https://www.gug-it.de/seiten/Warehouse/Kaufen.html" target="buy" title="Warehouse kaufen" style="color: #000;">Kaufen</a></p>
				</div>
			</div>
		</div>
		<div class="row" style="margin-top: 50px;">
			<div class="col-md-12">
				<p>
					Probieren Sie Warehouse kostenlos neunzig Tage lang aus. Wenn Sie Fragen, Kritiken, Anregungen
					oder Probleme haben, wenden Sie sich an uns. Wir helfen Ihnen gerne und stehen mit Rat und Tat
					zur Seite.
				</p>
			</div>
		</div>
	</div>
	<div id="videos" class="container-fluid section">
		<div class="container">
			<div class="row">
				<div class="col-xs-12">
					<h2>Installationsvideos</h2> 
					<p class="title_description_2">zu Ihrer Unterstützung</p>
					<p>Videos zu Warehouse finden Sie auf <a href="https://www.youtube.com/channel/UCc8FGMlCCJJr5OINddEIyig/videos">YouTube in unserem Channel.</a></p>
				</div>
			</div>
		</div>
	</div>
	<div id="more" class="container section">
		<div class="row">
			<div class="col-xs-12">
				<h2>Weitere Informationen</h2> 
				<p class="title_description_2">Lesen Sie mehr über Warehouse</p>
			</div>
		</div>

		<div class="row">
			<div class="col-md-12">
				<p>
					Weitere Informationen finden Sie auf unserer Website
					<a href="https://www.gug-it.de/seiten/Warehouse/Was%20ist%20Warehouse.html" title="Weitere Informationen zu Warehouse auf gug-it.de">www.gug-it.de</a>
					und in unserer
					<a href="https://www.gug-it.de/files/Unternehmensmappe.pdf" title="GuG-IT-Broschüre">Broschüre</a> auf Seite 2.
				</p>
			</div>
		</div>
	</div>

	<div id="news" class="container-fluid section">
		<div class="container">
			<div class="row">
				<div class="col-xs-12">
					<h2>Neuigkeiten</h2> 
					<p class="title_description_2">Wir entwickeln weiter</p>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<p><strong>Neue Funktionen in Version 1.8.8 vom 05.10.2017:</strong></p>
					<ul>
						<li><strong>Erste Beta-Version von Warehouse 1.8.8 für macOS Sierra und High Sierra.</strong></li>
						<li>Fehlerkorrektur: Übersetzte Adresse bei Aufträgen führt nicht mehr zum Absturz.</li>
					</ul>

					<p><strong>Neue Funktionen in Version 1.8.7 vom 04.10.2017:</strong></p>
					<ul>
						<li>Belege werden jetzt absteigend nach Datum sortiert und nicht mehr aufsteigend nach ID.</li>
						<li>Kategorien im Datei-Modul werden jetzt alphabetisch sortiert.</li>
						<li>Kleine Fehlerkorrekturen.</li>
					</ul>
					<p><span class="glyphicon glyphicon-download" aria-hidden="true"></span> <a href="https://www.gug-it.de/seiten/Warehouse/Kostenlos%20herunterladen.html" target="download">Laden Sie Warehouse hier kostenlos herunter.</a></p>
				</div>
			</div>
		</div>
	</div>

	<div id="macos" class="container-fluid section">
		<div class="container">
			<div class="row">
				<div class="col-xs-12">
					<h2>Warehouse für macOS Sierra und High Sierra</h2>
					<p class="title_description_2">Beta-Version</p>
				</div>
			</div>

			<div class="row">
				<div class="col-md-12">
					<p>
						Sie nutzen macOS? Dann haben wir sehr gute Neuigkeiten: Heute haben wir die
						erste Version von Warehouse für macOS Sierra und High Sierra veröffentlicht.
						Probieren Sie es aus!
					</p>
					<p style="padding-bottom: 50px;"><span class="glyphicon glyphicon-download" aria-hidden="true"></span> <a href="https://www.gug-it.de/seiten/Warehouse/Kostenlos%20herunterladen.html" target="download">Laden Sie Warehouse hier kostenlos herunter.</a></p>
				</div>
			</div>

			<div class="row">
				<div class="col-md-12" align="center">
					<h2 style="margin-bottom: 30px;">Warehouse</h2>
					<img src="images/Icon.png" title="Warehouse jetzt für macOS Sierra und High Sierra" alt="Warehouse-Logo">
				</div>
			</div>
			<div class="row">
				<div class="col-md-5" align="center">
					<img src="images/SierraFont.png" title="Warehouse jetzt für macOS Sierra und High Sierra" alt="Sierra-Logo">
					<br>
					<img src="images/Sierra.png" title="Warehouse jetzt für macOS Sierra und High Sierra" alt="Sierra-Logo">
				</div>
				<div class="col-md-2" align="center">
					<p style="font-size: 150px; margin-top: 100px;">
						<span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
					</p>
				</div>
				<div class="col-md-5" align="center">
					<img src="images/HighSierraFont.png" title="Warehouse jetzt für macOS Sierra und High Sierra" alt="High-Sierra-Logo">
					<br>
					<img src="images/HighSierra.png" title="Warehouse jetzt für macOS Sierra und High Sierra" alt="High-Sierra-Logo">
				</div>
			</div>
			<div class="row">
				<div class="col-md-12" align="center">
					<img src="images/Login.png" title="Login-Screen unter macOS Sierra" alt="Login-Screen" class="img-responsive" style="max-width: 100%;">
				</div>
			</div>
		</div>
	</div>

	<div class="container">
		<div class="row">
			<div class="col-xs-12">
				<h2>Spezialangebot</h2>
				<p class="title_description_2">für clevere Unternehmer</p>
			</div>
		</div>
		<div class="row">
			<div class="col-md-12">
				<p>
					Buchen Sie auf unserem Firmenportal den "Premiumeintrag 12" für Ihre Firma und erhalten Sie Warehouse kostenlos dazu.<br>
					<a href="https://www.compow.de/prices/premium" target="compow">
						<span class="glyphicon glyphicon-hand-right" aria-hidden="true" style="color: #f37f32; font-size: 35px; margin-right: 15px; margin-top: 15px;"></span>
						Für weitere Informationen hier klicken.
					</a>
				</p>
				<p style="padding-top: 30px;">
					<a href="https://www.compow.de" target="compow"><img src="images/Compow.gif" class="img-responsive" title="compow - Unternehmensportal, Branchenbuch und Stellenmarkt" alt="compow-Logo"></a>
				</p>
			</div>
		</div>
	</div>



	<div id="imprint" class="row-fluid">
		<footer class="nb-footer">
			<div class="container">
				<div class="row">
					<div class="col-md-3 col-sm-6">
						<div class="footer-info-single padding_right_20">
							<h3 class="title">Ein Projekt der</h3>
							<a href="https://www.gug-it.de/" target="_blank" title="Link zu GuG-IT GbR"><img src="images/GuG-IT-Logo.png" class="img-responsive center-block margin_top_20" alt="GuG-IT - IT-Dienstleister"></a>
						</div>
					</div>

					<div class="col-md-3 col-sm-6">
						<div class="footer-info-single">
							<h3 class="title">Adresse</h3>
							GuG-IT GbR - Olena &amp; Thorsten Geppert<br>
							Steinkaul 24<br>
							56651 Oberdürenbach<br>
							Verantwortlich:<br>
							Olena und Thorsten Geppert
						</div>
					</div>

					<div class="col-md-3 col-sm-6">
						<div class="footer-info-single">
							<h3 class="title">Kontakt</h3>
							<ul class="list-unstyled">
								<li class="text">Tel.: 02646 / 8453007</li>
								<li class="text">Mobil: 0151 / 22666988</li>
								<li class="text">Mail: <a href="mailto:info@gug-it.de">info@gug-it.de</a></li>
								<li class="text"><a href="http://www.facebook.com/GuGITGbR" title="Link zu Facebook">Facebook</a></li>
								<li class="text"><a href="https://twitter.com/GuG_IT_GbR" title="Link zu Twitter">Twitter</a></li>
							</ul>
						</div>
					</div>

					<div class="col-md-3 col-sm-6">
						<div class="footer-info-single">
							<h3 class="title">Weiteres</h3>
							<ul class="list-unstyled">
								<li class="text">USt-IdNr: DE299130475</li>
								<li class="text">Steuernummer: 01/049/31754</li>
								<li class="text">
									<a href="https://www.gug-it.de/seiten/Unternehmen/AGB.html" title="Link zu unseren allgemeinen Geschäftsbedingungen">AGB</a>
								</li>
								<li class="text">
									<a href="https://www.gug-it.de/seiten/Unternehmen/AGB.html#datenschutz" title="Link zur DatenschutzerklÃ¤rung">Datenschutz</a>
								</li>
								<li class="text"><a href="https://www.gug-it.de/seiten/Unternehmen/AGB.html#widerrufsrecht" title="Link zum Widerrufsrecht">Widerrufsrecht</a></li>
							</ul>
						</div>
					</div>
				</div>
			</div>
		</footer>
	</div>
	
	<script>
		document.addEventListener("DOMContentLoaded", function() { __Privacy(); });
	</script>
</body>
</html>

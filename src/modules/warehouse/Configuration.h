#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define VERSION_CHECK

#define NAME       _N("DynSoft.com - Thorsten Geppert - Warehouse 4")
#define SOFTWARE   _N("Warehouse")
#define VERSION    _N("4.0.0")
#define ENTERPRISE _N("DynSoft.com - Thorsten Geppert")
#define TITLE      ENTERPRISE + _N(" - ") + SOFTWARE

#define DEMO_SHOW         600000 // all ten Minutes
#define REMINDER_INTERVAL     60 // in Seconds

// TODO DS -- Wird nicht mehr genutzt
#define CONTACT_URL  _N("www.dynsoft.com")
#define CONTACT_FILE _N("/modules/contact/index.ds")
#define BANK_URL     _N("https://www.bundesbank.de/de/aufgaben/unbarer-zahlungsverkehr/serviceangebot/bankleitzahlen/download-bankleitzahlen-602592")

#define USER_CONFIGURATION_FILE _N(".DynSoftWarehouse.ini")

#define UPDATE_URL         _N("http://version.dynsoft.com/warehouse.conf")
#define CUSTOMERCENTER_URL _N("https://www.dynsoft.com/seiten/Kundencenter/index.html")
#define GUG_IT_URL        _N("https://www.dynsoft.com")

#ifdef __WXMAC__
	#define RESOURCE_PATH _N("Warehouse.app/Contents/Resources/")
#elif defined __WXMSW__
	#define RESOURCE_PATH _N("resources\\")
#else
	#define RESOURCE_PATH _N("resources/")
#endif

#ifdef __WXMSW__
	#define ICON() SetIcon(wxIcon(rit->GRP(_N("images"), _N("WarehouseIcon.ico")), wxBITMAP_TYPE_ICO))
#else
	#define ICON() SetIcon(wxIcon(WarehouseIcon_xpm))
#endif

#define GTC _N("<html><head><title>AGB</title></head><body><h1>Allgemeine Gesch&auml;ftsbedingungen</h1><p><strong>Liebe Besucherin, lieber Besucher,</strong></p><p>wir m&ouml;chten die Allgemeinen Gesch&auml;ftsbedingungen so kurz und einfach halten, wie es uns m&ouml;glich ist, da viele Umst&auml;nde entweder bereits gesetzlich geregelt sind oder, im Falle einer beidseitigen Gesch&auml;ftsbeziehung, ein genauer Vertrag geschlossen wird.</p><ol><li><strong>Kosten</strong><ol><li>Durch die Nutzung dieser Website entstehen keine Kosten, weder beim Besuch, noch w&auml;hrend oder nach der Registrierung, noch nach der Anmeldung im Kundencenter.</li><li>Auf entstehende Kosten wird explizit und gut sichtbar hingewiesen und sie m&uuml;ssen von Ihnen best&auml;tigt werden.</li></ol></li><li><strong>Produkte, Weitergaben, Nutzungsrechte</strong><ol><li>Alle hier angebotenen Produkte sind Eigentum der GuG-IT GbR - Olena und Thorsten Geppert.</li><li>Die Produkte, die nicht unter der BSD-Lizenz lizensiert sind, d&uuml;rfen nicht weitergegeben werden.</li><li>Die Produkte, die nicht unter der BSD-Lizenz lizensiert sind, d&uuml;rfen nicht ver&auml;ndert oder disassembliert oder dekompiliert werden.</li><li>Sie erhalten bei Vertragsabschluss die Nutzungsrechte f&uuml;r unsere Software, die nicht unter der BSD-Lizenz stehen, der Besitz bleibt bei GuG-IT GbR - Olena und Thorsten Geppert.</li><li>Weder Lizenznummern noch Anmeldeinformationen (Login-Daten) d&uuml;rfen von Ihnen an Dritte weitergegeben werden.</li><li>GuG-IT GbR - Olena und Thorsten Geppert &uuml;bernimmt keine Haftung f&uuml;r eventuell auftretende Sch&auml;den, Datenverluste oder Fehlberechnungen sowie andere Fehler wie Fehlverhalten der Produkte.</li></ol></li><li><strong><a name=\"datenschutz\"></a>Datenschutz</strong><ol><li>Ihre Daten werden nicht an Dritte weitergegeben, sofern dieser Umstand rechtlich nicht erforderlich ist.</li><li>Ihre Daten werden von uns nur im Umfang der Gesch&auml;ftsbeziehung genutzt.</li><li>Sie erhalten Newsletter, Telefonwerbung und postalische Werbung nur, sofern Sie in Ihrem Profil zugestimmt haben.</li><li>Cookies werden zur Identifikation des Benutzers f&uuml;r den Login der Website genutzt.</li></ol></li><li><strong><a name=\"widerrufsrecht\"></a>Widerrufsrecht</strong><ol><li>Wird einem Verbraucher durch Gesetz ein Widerrufsrecht nach dieser Vorschrift einger&auml;umt, so ist er an seine auf den Abschluss des Vertrags gerichtete Willenserkl&auml;rung nicht mehr gebunden, wenn er sie fristgerecht widerrufen hat. Der Widerruf muss keine Begr&uuml;ndung enthalten und ist in Textform oder durch R&uuml;cksendung der Sache innerhalb der Widerrufsfrist gegen&uuml;ber dem Unternehmer zu erkl&auml;ren; zur Fristwahrung gen&uuml;gt die rechtzeitige Absendung.</li><li>Die Widerrufsfrist betr&auml;gt 14 Tage, wenn dem Verbraucher sp&auml;testens bei Vertragsschluss eine den Anforderungen des &sect; 360 Abs. 1 entsprechende Widerrufsbelehrung in Textform mitgeteilt wird. Bei Fernabsatzvertr&auml;gen steht eine unverz&uuml;glich nach Vertragsschluss in Textform mitgeteilte Widerrufsbelehrung einer solchen bei Vertragsschluss gleich, wenn der Unternehmer den Verbraucher gem&auml;&szlig; Artikel 246 &sect; 1 Abs. 1 Nr. 10 des Einf&uuml;hrungsgesetzes zum B&uuml;rgerlichen Gesetzbuche unterrichtet hat. Wird die Widerrufsbelehrung dem Verbraucher nach dem gem&auml;&szlig; Satz 1 oder Satz 2 ma&szlig;geblichen Zeitpunkt mitgeteilt, betr&auml;gt die Widerrufsfrist einen Monat. Dies gilt auch dann, wenn der Unternehmer den Verbraucher &uuml;ber das Widerrufsrecht gem&auml;&szlig; Artikel 246 &sect; 2 Abs. 1 Satz 1 Nr. 2 des Einf&uuml;hrungsgesetzes zum B&uuml;rgerlichen Gesetzbuche zu einem sp&auml;teren als dem in Satz 1 oder Satz 2 genannten Zeitpunkt unterrichten darf.</li><li>Die Widerrufsfrist beginnt, wenn dem Verbraucher eine den Anforderungen des &sect; 360 Abs. 1 entsprechende Belehrung &uuml;ber sein Widerrufsrecht in Textform mitgeteilt worden ist. Ist der Vertrag schriftlich abzuschlie&szlig;en, so beginnt die Frist nicht, bevor dem Verbraucher auch eine Vertragsurkunde, der schriftliche Antrag des Verbrauchers oder eine Abschrift der Vertragsurkunde oder des Antrags zur Verf&uuml;gung gestellt wird. Ist der Fristbeginn streitig, so trifft die Beweislast den Unternehmer.</li><li>Das Widerrufsrecht erlischt sp&auml;testens sechs Monate nach Vertragsschluss. Diese Frist beginnt bei der Lieferung von Waren nicht vor deren Eingang beim Empf&auml;nger. Abweichend von Satz 1 erlischt das Widerrufsrecht nicht, wenn der Verbraucher nicht entsprechend den Anforderungen des &sect; 360 Abs. 1 &uuml;ber sein Widerrufsrecht in Textform belehrt worden ist, bei Fernabsatzvertr&auml;gen &uuml;ber Finanzdienstleistungen ferner nicht, wenn der Unternehmer seine Mitteilungspflichten gem&auml;&szlig; Artikel 246 &sect; 2 Abs. 1 Satz 1 Nr. 1 und Satz 2 Nr. 1 bis 3 des Einf&uuml;hrungsgesetzes zum B&uuml;rgerlichen Gesetzbuche nicht ordnungsgem&auml;&szlig; erf&uuml;llt hat.</li></ol></li></ol><p>&nbsp;</p><hr /><p>&nbsp;</p><h3><a name=\"bsd\"></a>BSD-Licence</h3><p>Copyright (c) 2011, GuG-IT GbR - Olena und Thorsten Geppert<br />All rights reserved.</p><p>Redistribution and use in source and binary forms, with or without  modification, are permitted provided that the following conditions are  met:</p><p style=\"padding-left: 30px;\">Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.<br />Redistributions  in binary form must reproduce the above copyright notice, this list of  conditions and the following disclaimer in the documentation and/or  other materials provided with the distribution.</p><p>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS  \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</p></body></html>")

#define DEFAULT_HOSTNAME _N("localhost")
#define DEFAULT_DATABASE _N("warehouse")
#define DEFAULT_USERNAME _N("postgres")
#define DEFAULT_PASSWORD _N("postgres")
#define DEFAULT_PORT     _N("5432")

#define FIRST_COLOUR           wxColour(_N("#f47e33"))
#define SECOND_COLOUR          wxColour(_N("#ffaf79"))
#define MANDATORY_FIELD_COLOUR wxColour(_N("#c64d00"))

#endif /* CONFIGURATION_H_ */

<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/xhtml/docbook.xsl"/>

	<xsl:param name="html.stylesheet" select="'style.css'"/>

	<xsl:param name="local.l10n.xml" select="document('')"/>
	<l:i18n xmlns:l="http://docbook.sourceforge.net/xmlns/l10n/1.0">
		<l:l10n language="en">
			<l:context name="xref">
				<l:template name="sect1" text="%t"/>
			</l:context>
		</l:l10n>
	</l:i18n>
</xsl:stylesheet>

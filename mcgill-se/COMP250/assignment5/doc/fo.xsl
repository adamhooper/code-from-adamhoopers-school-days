<?xml version="1.0"?>
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>

	<!-- general options -->
	<xsl:param name="paper.type">letter</xsl:param>

	<xsl:param name="page.margin.inner">0.5in</xsl:param>
	<xsl:param name="page.margin.outer">0.5in</xsl:param>
	<xsl:param name="page.margin.top">0.5in</xsl:param>
	<xsl:param name="page.margin.bottom">0.5in</xsl:param>

	<xsl:param name="title.margin.left">0in</xsl:param>

	<!-- Don't generate table of contents -->
	<xsl:param name="generate.toc"/>

	<!-- "expand-tabs"

	I wanted to print out assembly code in my PDF. However, assembly
	code often contains comments offset by tabs from the main text.
	Ideally, all of these comments would line up. But plain old fop
	just expands *all* tabs to 8 spaces. This was unacceptable to
	me.

	So I went and learned enough XSLT to expand tabs to the "right"
	number of spaces: 8 - (position of tab mod 8). It doesn't seem
	to be very efficient code-wise, but it ran on my 15-page
	document in no time, so I won't bother optimizing it.

	-->
	<xsl:template name="print-spaces">
		<xsl:param name="num"/>
		<xsl:if test="$num != 0">
			<xsl:text> </xsl:text>
			<xsl:call-template name="print-spaces">
				<xsl:with-param name="num" select="$num - 1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name="print-tab-expanded-line">
		<xsl:param name="text"/>
		<xsl:choose>
			<xsl:when test="contains($text, '&#x9;')">
				<xsl:value-of select="substring-before($text, '&#x9;')"/>

				<xsl:variable name="num-spaces" select="8 - (string-length(substring-before($text, '&#x9;')) mod 8)"/>
				<xsl:call-template name="print-spaces">
					<xsl:with-param name="num" select="$num-spaces"/>
				</xsl:call-template>

				<xsl:call-template name="expand-tabs">
					<xsl:with-param name="text" select="substring-after($text, '&#x9;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$text"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="expand-tabs">
		<xsl:param name="text"/>
		<xsl:choose>
			<xsl:when test="contains($text, '&#xa;')">
				<xsl:call-template name="expand-tabs">
					<xsl:with-param name="text" select="substring-before($text, '&#xa;')"/>
				</xsl:call-template>
				<xsl:text>&#xa;</xsl:text>
				<xsl:call-template name="expand-tabs">
					<xsl:with-param name="text" select="substring-after($text, '&#xa;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="print-tab-expanded-line">
					<xsl:with-param name="text" select="$text"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- Apply "expand-tabs" to programlisting tags -->
	<xsl:template match="programlisting"
		xmlns:fo="http://www.w3.org/1999/XSL/Format">
		<fo:block
			wrap-option="no-wrap"
			white-space-collapse="false"
			linefeed-treatment="preserve"
			text-align="start"
			space-before.minimum="0.8em"
			space-before.optimum="1em"
			space-before.maximum="1.2em"
			space-after.minimum="0.8em"
			space-after.optimum="1em"
			space-after.maximum="1.2em"
			font-family="monospace">

			<xsl:call-template name="expand-tabs">
				<xsl:with-param name="text" select="."/>
			</xsl:call-template>
		</fo:block>
	</xsl:template>
</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<!-- XSL Stylesheet to transform kvtml-2 files to xhtml in card layout -->
<xsl:stylesheet
  version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://www.w3.org/1999/xhtml">

  <xsl:output
    method="xml"
    encoding="UTF-8"
    standalone="no"
    omit-xml-declaration="no"
    doctype-public="-//W3C//DTD XHTML 1.1//EN"
    doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"
    indent="yes" />

  <!-- globale variables -->
  <xsl:variable
    name="cards_per_page"
    select="number(10)" />
  <xsl:variable
    name="pages"
    select="ceiling(count(/kvtml/entries/entry) div $cards_per_page)" />
  <xsl:variable
    name="cards"
    select="count(/kvtml/entries/entry)" />

  <xsl:template match="/">
    <html xmlns="http://www.w3.org/1999/xhtml">
      <head>
        <title>
          <xsl:value-of select="kvtml/information/title" />
        </title>
        <meta
          http-equiv="content-type"
          content="text/html; charset=UTF-8" />
        <meta
          http-equiv="Content-Style-Type"
          content="text/css" />
        <meta name="description" content="{kvtml/information/title}" />
        <style>
          /* A4 format for the page*/
          @page {
            size: 21.0cm 29.7cm;
            margin: 2cm;
          }
          
          body {
            margin: 0;
            padding: 0;
            color: #000;
            background-color: #fff;
            font: normal normal bold medium 'Arial', 'Helvetica' sans-serif;
            text-align: center;
            vertical-align: top;
          }
          
          table,td {
            table-layout: fixed;
            border-collapse: collapse;
            border-spacing: 0;
            border: thin dashed #000;
            empty-cells: hide;
            text-align: center;
          }
          
          table {
            margin: auto;
            margin-top: 0.5cm;
          }
          
          td {
            padding: 0cm;
            direction:ltr;
            text-align: left;
            vertical-align: top;
            /* A8 format for the flash cards*/
            width: 7.4cm;
            height: 5.2cm;
          }

          img {
            margin: 0cm 0.3cm 0.3cm 0.3cm;
            padding: 0;
            border: 2px;
            float: right;
            text-align: right;
            vertical-align: top;
          }
          
          .lesson {
            margin: 0.3cm 0.3cm 0.3cm 0.3cm;
            padding: 0;
            font-size : 0.7em;
            font-weight: normal;
            text-align: left;
            vertical-align: top;
          }

          .entry {
            margin: 1.5cm 0.3cm 0.3cm 0.3cm;
            padding: 0;
            text-align: center;
            vertical-align: middle;
          }

          .example {
            margin: 0.3cm 0.3cm 0.3cm 0.3cm;
            padding: 0.3cm 0 0 0;
            border-top: 1px solid #000;
            font-weight: normal;
            text-align: center;
            vertical-align: middle;
          }

          .pronunciation {
            color: #00f;
          }

        </style>
      </head>
      <body>
        <xsl:call-template name="all">
          <xsl:with-param
            name="i"
            select="$pages" />
        </xsl:call-template>
      </body>
    </html>
  </xsl:template>

  <xsl:template name="all">
    <xsl:param name="i" />
    <xsl:choose>
      <xsl:when test="$i &gt; 0">
        <!-- make front -->
        <xsl:call-template name="page">
          <xsl:with-param
            name="index"
            select="$pages - $i + 1" />
          <xsl:with-param
            name="side"
            select="0" />
        </xsl:call-template>
        <!-- make back -->
        <xsl:call-template name="page">
          <xsl:with-param
            name="index"
            select="$pages - $i + 1" />
          <xsl:with-param
            name="side"
            select="1" />
        </xsl:call-template>
        <xsl:call-template name="all">
          <xsl:with-param
            name="i"
            select="$i - 1" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="break" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="break"></xsl:template>

  <xsl:template name="page">
    <xsl:param name="index" />
    <xsl:param name="side" />
    <table>
      <xsl:choose>
        <!-- set attributes for front -->
        <xsl:when test="$side = 0">
          <xsl:attribute name="id">Front_<xsl:value-of select="$index" />
          </xsl:attribute>
          <xsl:attribute name="style">page-break-after:always; direction:ltr;</xsl:attribute>
        </xsl:when>
        <!-- set attributes for back (with direction:rtl the order of the cards will be
          changed and on the dopple-sided print out you'll have the rigth back on the falsh card -->
        <xsl:otherwise>
          <xsl:attribute name="id">Back_<xsl:value-of select="$index" />
          </xsl:attribute>
          <xsl:attribute name="style">page-break-after:always; direction:rtl;</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <!-- calculate number of flash cards on normal page / last page -->
      <xsl:variable name="current_cards">
        <xsl:choose>
          <!-- last page -->
          <xsl:when
            test="$index = $pages and ($cards mod $cards_per_page) !=0">
            <xsl:value-of select="$cards mod $cards_per_page" />
          </xsl:when>
          <!-- normal page -->
          <xsl:otherwise>
            <xsl:value-of select="$cards_per_page" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:call-template name="row">
        <xsl:with-param
          name="j"
          select="$current_cards" />
        <xsl:with-param
          name="index"
          select="$index" />
        <xsl:with-param
          name="side"
          select="$side" />
      </xsl:call-template>
    </table>
  </xsl:template>

  <xsl:template name="row">
    <xsl:param name="j" />
    <xsl:param name="index" />
    <xsl:param name="side" />
    <xsl:choose>
      <xsl:when test="$j &gt; 0">
        <xsl:variable
          name="pos"
          select="$j + ($index - 1) * $cards_per_page" />
        <tr>
          <td>
            <xsl:call-template name="flashcard">
              <xsl:with-param
                name="pos"
                select="$j + ($index - 1) * $cards_per_page" />
              <xsl:with-param
                name="side"
                select="$side" />
            </xsl:call-template>
          </td>
          <td>
            <xsl:if test="$j != 1">
              <xsl:call-template name="flashcard">
                <xsl:with-param
                  name="pos"
                  select="$j - 1 + ($index - 1) * $cards_per_page" />
                <xsl:with-param
                  name="side"
                  select="$side" />
              </xsl:call-template>
            </xsl:if>
          </td>
        </tr>
        <xsl:call-template name="row">
          <xsl:with-param
            name="j"
            select="$j - 2" />
          <xsl:with-param
            name="index"
            select="$index" />
          <xsl:with-param
            name="side"
            select="$side" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="break" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="flashcard">
    <xsl:param name="pos" />
    <xsl:param name="side" />
      <img
        src="hi48-app-parley.png"
        alt="Logo" />
      <p class="lesson">
        <xsl:value-of
          select="/kvtml/lessons/container/name[../entry/@id=/kvtml/entries/entry[$pos]/@id]" />
      </p>
      <xsl:variable
        name="pronunciation"
        select="/kvtml/entries/entry[$pos]/translation[@id=$side]/pronunciation" />
      <p class="entry">
        <xsl:value-of
          select="/kvtml/entries/entry[$pos]/translation[@id=$side]/text" />
        <xsl:if test="$pronunciation">
          <span class="pronunciation">
            [<xsl:value-of select="$pronunciation" />]
          </span>
        </xsl:if>
      </p>
      <xsl:variable
        name="example"
        select="/kvtml/entries/entry[$pos]/translation[@id=$side]/example" />
      <xsl:if test="$example">
        <p class="example">
          <xsl:value-of select="$example" />
        </p>
      </xsl:if>
  </xsl:template>

</xsl:stylesheet>
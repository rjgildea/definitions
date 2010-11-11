<?xml version="1.0" encoding="UTF-8"?>

<!--
    ########### SVN repository information ###################
    # $LastChangedDate: 2010-10-07 05:35:23 +0100 (Thu, 07 Oct 2010) $
    # $LastChangedBy: Freddie Akeroyd $
    # $LastChangedRevision: 601 $
    # $HeadURL: http://svn.nexusformat.org/definitions/trunk/xslt/nxdl2sch.xsl $
    ########### SVN repository information ###################
    
    Purpose:
    This stylesheet is used to translate the NeXus Definition Language
    specifications into a sample XML file

    Usage (for example NXsource class):
        xsltproc nxdl2ns.xsl NXmonopd.nxdl.xml > testfile.xml
    the run nxconvert to get hdf5
-->

<!-- 
    This stylesheet automatically add the contents of nexus_base.sch to the final output
-->

<xsl:stylesheet
    version="1.0"
    xmlns:nxdl="http://definition.nexusformat.org/nxdl/3.1"
    xmlns:nxsd="http://definition.nexusformat.org/schema/3.1"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    <xsl:output method="xml" indent="yes" version="1.0" encoding="UTF-8"/>
    
    <xsl:template match="/">
        <xsl:comment>Generated by $Id: nxdl2sch.xsl 601 2010-10-07 04:35:23Z Freddie Akeroyd $</xsl:comment>
        
        <xsl:element name="NXroot">
            <xsl:attribute name="NeXus_version">4.1.0</xsl:attribute>
            <xsl:attribute name="XML_version">mxml</xsl:attribute>
            <xsl:attribute name="file_name">test.xml</xsl:attribute>
            <xsl:attribute name="file_time">2009-10-27 09:17:28+0100</xsl:attribute>
            <xsl:apply-templates select="*" />
         </xsl:element>
    </xsl:template>
    
    <!-- output any documentation as comment in nexus file 
         alternatively, we could output as an atttribute or field in the final nexus file
     -->
    <xsl:template match="nxdl:doc">
        <!--xsl:comment> <xsl:value-of select="."/> </xsl:comment-->
    </xsl:template>
    
    <xsl:template match="nxdl:definition">
        <xsl:variable name="svnid" select="@svnid" />
        <!--xsl:comment>Processing <xsl:value-of select="@category"/> definition <xsl:value-of select="@name"/>, version <xsl:value-of select="@version"/>(<xsl:value-of select="$svnid"/>)</xsl:comment-->

	<!-- add an NXentry if no top level group present -->
         <xsl:choose>
         <xsl:when test="nxdl:group[@type='NXentry']" >
         <xsl:apply-templates select="*" />
         </xsl:when>
         <xsl:otherwise>
         <xsl:element name="NXentry">
         <xsl:attribute name="name">entry</xsl:attribute>
         <xsl:apply-templates select="*" />
         </xsl:element>
         </xsl:otherwise>
         </xsl:choose>

    </xsl:template>

    <xsl:template match="nxdl:group">
        <xsl:variable name="type" select="@type"/>
        <xsl:element name="{$type}">            
            <xsl:attribute name="name">
                <xsl:choose>
                    <xsl:when test="@name"><xsl:value-of select="@name"/></xsl:when>
                    <xsl:otherwise><xsl:value-of select="substring-after($type,'NX')"/></xsl:otherwise>
                </xsl:choose>
            </xsl:attribute>
            <xsl:for-each select="@*">
                <xsl:choose>
                    <xsl:when test="name() = 'type'">
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:variable name="var" select="name()" />
                        <xsl:attribute name="{$var}">
                            <xsl:value-of select="."/>
                        </xsl:attribute>                        
                    </xsl:otherwise>
                </xsl:choose>                
            </xsl:for-each>
        <xsl:apply-templates select="*" />
        </xsl:element>
    </xsl:template>
    
    <xsl:template match="nxdl:field">
        <xsl:variable name="name" select="@name"></xsl:variable>
        <xsl:element name="{$name}">
            <xsl:for-each select="@*">
                <xsl:choose>
                    <xsl:when test="name() = 'name'" />
                    <xsl:when test="name() = 'type'">
                        <xsl:attribute name="NAPItype">
                            <xsl:call-template name="translate_type">
                                <xsl:with-param name="type" select="."/>
                            </xsl:call-template>
                         </xsl:attribute>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:variable name="var" select="name()" />
                        <xsl:attribute name="{$var}">
                            <xsl:value-of select="."/>
                        </xsl:attribute>                        
                    </xsl:otherwise>
                </xsl:choose>                
            </xsl:for-each>
            <xsl:choose>
                <xsl:when test="nxdl:enumeration">
                    <xsl:apply-templates select="nxdl:enumeration"></xsl:apply-templates>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:call-template name="generate_data">
                        <xsl:with-param name="type" select="@type"/>
                    </xsl:call-template>                    
                </xsl:otherwise>
            </xsl:choose>
        </xsl:element> 
    </xsl:template>   

    <!-- select fist value of an enumeration as a sample value and set NAPItype attribute -->
    <xsl:template match="nxdl:enumeration">
        <xsl:variable name="enumvalue" select="nxdl:item[1]/@value"/>
        <!--xsl:attribute name="NAPItype">NX_CHAR[<xsl:value-of select="string-length($enumvalue)"/>]</xsl:attribute-->
        <xsl:value-of select="$enumvalue"/>
    </xsl:template>
    
    
    <!-- return a sequence of valid types for an NXDL type -->
    <xsl:template name="translate_type">
        <xsl:param name="type" select="NX_CHAR"/>
        <xsl:choose>
            <xsl:when test="$type = 'NX_NUMBER'">NX_FLOAT32[1]</xsl:when>
            <xsl:when test="$type = 'NX_FLOAT'">NX_FLOAT32[1]</xsl:when>
            <xsl:when test="$type = 'NX_INT'">NX_INT32[1]</xsl:when>
            <xsl:when test="$type = 'NX_UINT'">NX_UINT32[1]</xsl:when>
            <xsl:otherwise>NX_CHAR</xsl:otherwise>
        </xsl:choose>
    </xsl:template>        
 
    <xsl:template name="generate_data">
        <xsl:param name="type" select="NX_CHAR"/>
        <xsl:choose>
            <xsl:when test="$type = 'NX_NUMBER'">1.0</xsl:when>
            <xsl:when test="$type = 'NX_FLOAT'">1.0</xsl:when>
            <xsl:when test="$type = 'NX_INT'">1.0</xsl:when>
            <xsl:when test="$type = 'NX_UINT'">1.0</xsl:when>
            <xsl:otherwise>NX_CHAR</xsl:otherwise>
        </xsl:choose>
    </xsl:template>        
    
</xsl:stylesheet>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output omit-xml-declaration="yes"/>
<xsl:output method="text"/>
<xsl:strip-space elements="*" />

<xsl:template match="text()">
<xsl:value-of select="."/>
</xsl:template>

<xsl:template match="and">&amp;&amp;</xsl:template>
<xsl:template match="le">&lt;=</xsl:template>
<xsl:template match="ge">&gt;=</xsl:template>

<xsl:template match="@save-state">
	state_old=state_current;
</xsl:template>

<xsl:template match="@next">
	state_current=State::<xsl:value-of select="."/>;
</xsl:template>

<xsl:template match="@exclusive">
	break;
</xsl:template>

<xsl:template match="transition">
	<xsl:apply-templates select="./node()"/>
		{
		<xsl:apply-templates select="./@save-state "/>
		<xsl:apply-templates select="./@next" />
		<xsl:apply-templates select="./@exclusive" />
		}
</xsl:template>

<xsl:template match="case">
	case <xsl:value-of select="@input"/>:
		{
		<xsl:apply-templates select="./text()" />
		<xsl:apply-templates select="./@save-state "/>
		<xsl:apply-templates select="@next" />
		<xsl:apply-templates select="transition"/>
		}
	break;
</xsl:template>

<xsl:template match="default">
default:
	{
	<xsl:apply-templates select="./text()" />
	<xsl:apply-templates select="./@save-state "/>
	<xsl:apply-templates select="@next" />
	<xsl:apply-templates select="transition"/>
	}
</xsl:template>

<xsl:template match="state">
case State::<xsl:value-of select="@name" />:
	switch(<xsl:value-of select="../inputvar/@name" />)
		{
		<xsl:apply-templates />
		}
	break;
</xsl:template>

<xsl:template match="actions">
switch(state_current)
	{
	<xsl:apply-templates/>
	}
</xsl:template>

<xsl:template match="statevar">
	<xsl:value-of select="@type" /><xsl:text> </xsl:text><xsl:value-of select="@name" />;

</xsl:template>
<xsl:template match="inputvar">
	<xsl:value-of select="@type" /><xsl:text> </xsl:text><xsl:value-of select="@name" />
</xsl:template>

<xsl:template match="statemachine">
class <xsl:value-of select="@name" />
	{
	private:
		enum class State:int
			{
			<xsl:for-each select="state">
				<xsl:value-of select="@name" />,
			</xsl:for-each>
			END
			};

		State state_current;
		<xsl:apply-templates select="statevar" />
	};
void <xsl:value-of select="@name" />::operator()(<xsl:apply-templates select="inputvar" />)
	{
	switch(state_current)
		{
		<xsl:apply-templates select="state"/>
		}
	}
</xsl:template>

<xsl:template match="namespace">
namespace <xsl:value-of select="@name" />
	{
	<xsl:apply-templates />
	}
</xsl:template>

</xsl:stylesheet>

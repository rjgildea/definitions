.. $Id$

<?xml version="1.0" encoding="UTF-8"?>
<?oxygen RNGSchema="http://www.oasis-open.org/docbook/xml/5.0/rng/docbook.rng" type="xml"?>
<!-- # $Id$ -->
<section xml:id="Fileformat" 
    version="5.0"
    xmlns="http://docbook.org/ns/docbook" 
    xmlns:xlink="http://www.w3.org/1999/xlink" 
    xmlns:xi="http://www.w3.org/2001/XInclude"
  >
    <title>Physical File format</title>
    <para>
      This section describes how NeXus structures are mapped to features of the underlying 
      physical file format.
      <indexterm>
        <primary>NeXus</primary>
        <secondary>low-level file formats</secondary>
      </indexterm> 
      This is a guide for people who wish to create NeXus files without 
      using the NeXus-API.
      <indexterm>
        <primary>NAPI</primary>
        <secondary>bypassing</secondary>
      </indexterm>
    </para>

    <section xml:id="Fileformat-HDF-Choice">
        <title>Choice of HDF as Underlying File Format</title>
        <para> At its beginnings, the founders of NeXus identified the 
            Hierarchical Data Format (HDF), 
            <indexterm>
                <primary>HDF</primary>
            </indexterm> initially from the National Center for Supercomputing Applications (NCSA)
            at the University of Illinois at Urbana-Champaign (UIUC) and later spun off into its own
            group called The HDF Group (THG),<footnote>
                <para> The HDF Group: <link xlink:href="http://www.hdfgroup.org/">http://www.hdfgroup.org/</link>
                </para>
            </footnote> as a multi-platform data storage format with capacity for conveying large
            data payloads and a substantial user community. HDF (now HDF5) was provided with
            software to read and write data (this is the application-programmer interface, or API) <indexterm>
                <primary>API</primary>
            </indexterm> using a large number of computing systems in common use for neutron and
            X-ray science. HDF is a binary data file format that supports compression and structured
            data.
        </para>
    </section>

  <section xml:id="Fileformat-Mapping-HDF">
    <title>Mapping NeXus into HDF</title>
    <para>
      NeXus data structures map directly to HDF structures.
      NeXus <emphasis>groups</emphasis> are HDF4 <emphasis>vgroups</emphasis> or HDF5 <emphasis>groups</emphasis>, 
      NeXus data sets (or <emphasis>fields</emphasis>) are HDF4 <emphasis>SDS (scientific data sets)</emphasis>
      or HDF5 <emphasis>datasets</emphasis>.
      Attributes map directly to HDF group or dataset attributes. 
      <indexterm>
        <primary>attributes</primary>
      </indexterm>
    </para>
    <para>
      The only special case is the NeXus class name. HDF4 supports a group class 
      which is set with the <code>Vsetclass()</code> call 
      and read with <code>VGetclass()</code>. 
       HDF-5 has no group class. Thus the NeXus class 
       is stored as a group attribute with the name <code>NX_class</code>. 
    </para>
    <para>
      NeXus links directly map to the HDF linking mechanisms. 
    </para>


  </section>

  <section xml:id="Fileformat-Mapping-XML">
    <title>Mapping NeXus into XML</title>
    <para>
      This takes a bit more work than HDF. 
      At the root of NeXus XML file 
      is a XML element with the name <code>NXroot</code>. 
      Further XML attributes to 
      <indexterm>
        <primary>attributes</primary>
      </indexterm> 
      <code>NXroot</code> define the NeXus file level attributes.
    </para>
    <para>
      NeXus groups are encoded into XML as elements with the 
      name of the NeXus class and an XML attribute <code>name</code> which defines the 
      NeXus name of the group. Further group attributes become XML attributes. An example:
      <example>
        <title>NeXus group element in XML</title>
        <programlisting language="xml" linenumbering="numbered"
              ><xi:include href="examples/mapping1.xml.txt" parse="text"
              /></programlisting>
      </example>
    </para>
    <para>
      NeXus data sets are encoded as XML elements with 
      the name of the data. An attribute <code>NAPItype</code> defines the type and 
      dimensions
      <indexterm><primary>dimension</primary></indexterm>
      of the data. The actual data is 
      stored as <code>PCDATA</code><footnote><para
        ><code>PCDATA</code> is the XML term for 
      		<emphasis>parsed character data</emphasis> (see: <link
              xlink:href="http://www.w3schools.com/xml/xml_cdata.asp"
              >
      		<code>http://www.w3schools.com/xml/xml_cdata.asp</code
      		></link>).</para></footnote> in the element. Another example:
      <example>
        <title>NeXus data elements</title>
        <programlisting language="xml" linenumbering="numbered"
              ><xi:include href="examples/mapping2.xml.txt" parse="text"
              /></programlisting>
      </example>
      Data are printed in appropriate formats and in C storage order. 
      The codes understood for <code>NAPItype</code> are 
      all the NeXus data type names. The dimensions
      <indexterm><primary>dimension</primary></indexterm>
      are given in square brackets as a comma 
      separated list. No dimensions need to be given if 
      the data is just a single value. 
      Data attributes are represented as XML attributes.  
      <indexterm>
        <primary>attributes</primary>
      </indexterm>
      If the attribute is not a text string, then the 
      attribute is given in the form: <emphasis>type:value</emphasis>, for example: 
      <code>signal="NX_INT32:1"</code>.  
    </para>
    <para>
      NeXus links 
      <indexterm><primary>link</primary></indexterm>
      are stored in XML as XML elements 
      with the name <code>NAPIlink</code> 
      <indexterm><primary>NAPIlink</primary></indexterm>
      and a XML attribute 
      <code>target</code> which stores the path to the linked 
      entity in the file.  If the item is linked under 
      a different name, then this name is specified as a XML attribute name to 
      the element <code>NAPIlink</code>. 
    </para>
    <para>
      The authors of the NeXus API worked with the author of the miniXML XML library to 
       create a reasonably efficient way of handling numeric data with XML. Using the NeXus API handling 
      something like 400 detectors versus 2000 time channels in XML is not a problem. But you may 
      hit limits with XML as the file format when data becomes to large or you try to process NeXus
      XML files with general XML tools. General XML tools are normally ill prepared to process large 
      amounts of numbers. 
    </para>
    </section>

  <section xml:id="Fileformat-SpecialAttributes">
    <title>Special Attributes</title>
    <para>
      NeXus makes use of some special attributes for its internal purposes. 
      <indexterm>
        <primary>attributes</primary>
      </indexterm>
      These attributes are stored as normal group or data set attributes 
      in the respective file format. These are:
      <variablelist>
        <varlistentry><term>target</term>
          <listitem><para>
            <indexterm><primary>link</primary></indexterm>
            This attribute is automatically created when items get linked. 
            The target attribute contains a text string with 
            the path to the source of the item linked. 
          </para></listitem>
        </varlistentry>
        <varlistentry><term><code>napimount</code></term>
          <listitem><para>
            The <code>napimount</code> attribute is used to implement
            external linking in NeXus. 
            The string is a URL to the file and group in the 
            external file to link too. The system is meant to be extended. 
            But as of now, the only format supported is: 
            <code>nxfile://path-to-file#path-infile</code>. 
            This is a NeXus file in the file system at path-to-file 
            and the group path-infile in that 
            NeXus file.  
          </para></listitem>
        </varlistentry>
        <varlistentry><term><code>NAPIlink</code></term>
          <listitem><para>
            <indexterm><primary>NAPIlink</primary></indexterm>
            NeXus supports linking items in another group under another name. 
            This is only supported natively in HDF-5. 
            For HDF-4 and XML a crutch is needed. 
            This crutch is a special class name or attribute 
            <code>NAPIlink</code> combined with the 
            target attribute. For groups, <code>NAPILink</code> 
            is the group class, for data items a special attribute 
            with the name <code>NAPIlink</code>.  
          </para></listitem>
        </varlistentry>
      </variablelist>
    </para>
  </section>
</section>
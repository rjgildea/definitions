.. $Id$

<?xml version="1.0" encoding="UTF-8"?>
<?oxygen RNGSchema="http://www.oasis-open.org/docbook/xml/5.0/rng/docbook.rng" type="xml"?>
<!-- # $Id$ -->
<section  xml:id="Rules" 
    version="5.0"
    xmlns="http://docbook.org/ns/docbook" 
    xmlns:xlink="http://www.w3.org/1999/xlink" 
    xmlns:xi="http://www.w3.org/2001/XInclude"
  >
    <title>Rules for Structuring Information in NeXus Files</title>
    <para>
      <indexterm>
        <primary>rules</primary>
        <secondary>NeXus</secondary>
      </indexterm>
       All NeXus files contain one or many groups of type <code>NXentry</code> at root level.
       Many files contain only one 
       <code>NXentry</code>
       group, then the name is <code>entry</code>.
       The NXentry level of hierarchy is there to support the storage of multiple related experiments in one file. 
       Or to allow the NeXus file to serve as a container for storing a whole scientific workflow from data acquisition to
       publication ready data. 
        Also, <code>NXentry</code> class 
       groups can contain raw data or processed data. 
        For files with more than one <code>NXentry</code> group, since HDF requires
        that no two items at the same level in an HDF file may have the same name,
        the NeXus fashion is to
        assign names with an incrementing index appended, such as 
        <code>entry1</code>, <code>entry2</code>, <code>entry3</code>, etc.
      <indexterm>
        <primary>rules</primary>
        <secondary>naming</secondary>
      </indexterm>
      </para>
      
      <para>
        In order to illustrate what is written in the text, example hierarchies like the one in 
        figure <link xlink:href="#table.RawData">Raw Data</link> are provided.
      </para>
      
    <section xml:id="Rules-NXentry-raw-data">
      <title>Content of a Raw Data <code>NXentry</code> Group</title>
      <para>An example raw data hierarchy is 
          shown in figure <link xlink:href="#table.RawData">Raw Data</link> 
          (only showing the relevant parts of the data hierarchy).
          In the example shown, the <code>data</code> field in the <code>NXdata</code> group
          is linked to the 2-D detector data (a 512x512 array of 32-bit integers)
          which has the attribute <code>signal=1</code>.
        Note that <code>[,]</code> represents a 2D array.
      </para>
      <figure xml:id="table.RawData">
        <title>NeXus Raw Data Hierarchy</title>
        <programlisting linenumbering="numbered"
          ><xi:include href="examples/hierarchy-raw.txt" parse="text"
          /></programlisting>
      </figure>     

        <para>
          An <code>NXentry</code> describing raw data contains at least a <code>NXsample</code>, 
          one <code>NXmonitor</code>, 
          one <code>NXdata</code> and a <code>NXinstrument</code> group. 
          It is good practice to use the names 
          <code>sample</code> for the <code>NXsample</code> group, 
          <code>control</code> for the <code>NXmonitor</code> group holding the 
           experiment controlling monitor and 
           <code>instrument</code> for the <code>NXinstrument</code> group. 
          The <code>NXinstrument</code> group contains further groups describing the individual 
           components of the instrument as appropriate. 
        </para>
        <para>
          The <code>NXdata</code> group contains links to all those data 
          items in the <code>NXentry</code> hierarchy
          <indexterm>
            <primary>hierarchy</primary>
          </indexterm> 
          which are required to put up a default plot of the data.  
           As an example consider 
           a SAXS instrument with a 2D detector. 
           The <code>NXdata</code> will then hold a link to 
           the detector image.  
           If there is only one <code>NXdata</code> group,
           it is good practice to name it <code>data</code>. 
           Otherwise, the name 
           of the detector bank represented is a good selection.
        </para>
    </section>
        
    <section xml:id="Rules-NXentry-processed-data">
      <title>Content of a processed data <code>NXentry</code> group</title>
      <para>Processed data, see figure  <link xlink:href="#table.ProcessedData">Processed Data</link>,  
           in this context means the results of a data reduction or 
           data analysis program. Note that <code>[]</code> represents a 1D array.</para>
      
     <figure xml:id="table.ProcessedData">
       <title>NeXus Processed Data Hierarchy</title>
       <programlisting linenumbering="numbered"
          ><xi:include href="examples/hierarchy-processed.txt" parse="text"
          /></programlisting>
    </figure>     
      
        <para>
           NeXus stores such data in a simplified 
          <code>NXentry</code> structure. A processed data <code>NXentry</code>
          has at minimum a <code>NXsample</code>, 
          a <code>NXdata</code> and a <code>NXprocess</code> group. 
          Again the preferred name for the <code>NXsample</code> 
           group is <code>sample</code>. 
           In the case of processed data, the <code>NXdata</code> group holds the 
           result of the processing together with the associated axis data. 
           The <code>NXprocess</code> 
           group holds the name and version of the program used for this processing 
           step and further <code>NXparameter</code> groups. These groups ought to contain the 
           parameters used for this data processing step in suitable detail so that 
           the processing step can be reproduced. 
        </para>
        <para>
          Optionally a processed data <code>NXentry</code> 
          can hold a <code>NXinstrument</code> group with 
           further groups holding relevant information about the instrument. The 
           preferred name is again <code>instrument</code>. Whereas for a raw data file, NeXus 
           strives to capture as much data as possible, a <code>NXinstrument</code> group for 
           processed data may contain a much-reduced subset. 
        </para>         
    </section>
   
   <section xml:id="Rules-Subentry">
     <title><literal>NXsubentry</literal> or Multi-Method Data</title>
     <para>
        Especially at synchrotron facilities, there are experiments which perform several different methods  
        on the sample at the same time. For example, combine a powder diffraction experiment with XAS. 
        This may happen in the same scan, so the data needs to be grouped together. A suitable <literal>NXentry</literal> 
        would need to adhere to two different application definitions. This leads to name clashes which cannot be 
        easily resolved. In order to solve this issue, the following scheme was implemented in NeXus:
        <itemizedlist>
          <listitem><para>The complete beamline (all data) is stored in an 
            appropriate hierarchy in an <literal>NXentry</literal>.</para></listitem>
        <listitem><para>
          The <literal>NXentry</literal> group contains further <literal>NXsubentry</literal> groups, one for each method. The <literal>NXsubentry</literal> group is constructed 
          like a <literal>NXentry</literal> group. It contains links to all those data items required to fulfill the application definition 
          for the particular method it represents.  
        </para></listitem>
        </itemizedlist>
       See figure <link xlink:href="#table.NXsubentry">NeXus Multi Method Hierarchy</link> for an example hierarchy.
        Note that <code>[,]</code> represents a 2D array.
     </para>
   
      <figure xml:id="table.NXsubentry">
        <title>NeXus Multi Method Hierarchy</title>
        <programlisting linenumbering="numbered"
          ><xi:include href="examples/hierarchy-subentry.txt" parse="text"
          /></programlisting>
    </figure>     
   
   
   </section>
   
  <section xml:id="Rules-SpecialCases">
    <title>Rules for Special Cases</title>
      
    <section xml:id="Rules-SpecialCases-Scans">
      <title>Scans</title>
      <para>
         Scans are difficult to capture because they have great variety. Basically, 
         any variable can be scanned. Such behaviour cannot be captured in application definitions. 
         Therefore NeXus solves this difficulty with a set of 
         rules. In this section, <code>NP</code> is used as a symbol for the number of scan 
         points.
        <itemizedlist>
          <listitem>
            <para>
              The scan dimension <code>NP</code> is always the first dimension of any 
              multi-dimensional dataset. The reason for this is that HDF allows the first 
              dimension of a dataset to be unlimited.
              <!-- TODO "1st dimension only is unlimited": Isn't this an HDF4 rule, relaxed in HDF5? -->
              Which means, that data can be 
              appended to the dataset during the scan. 
            </para>
          </listitem>
          <listitem>
            <para>
              All data is stored as arrays of dimensions <code>NP</code>, original dimensions 
              of the data at the appropriate position in the <code>NXentry</code> hierarchy.
              <indexterm>
                <primary>hierarchy</primary>
              </indexterm>
            </para>
          </listitem>
          <listitem>
            <para>
              The <code>NXdata</code> group has to contain links to all variables varied during 
               the scan and the detector data. Thus the <code>NXdata</code> group  mimics the usual 
               tabular representation of a scan.
            </para>
          </listitem>
          <listitem>
            <para>
              Datasets in an <code>NXdata</code> group must contain the proper attributes
              to enable the default plotting,  
              <indexterm>
                <primary>NeXus basic motivation</primary>
                <secondary>default plot</secondary>
              </indexterm>
              as described in the section titled
              <link xlink:href="#NXdata-facilitates-TheDefaultPlot"><literal>NXdata</literal> 
                Facilitates Automatic Plotting</link>.
            </para>
          </listitem>
         </itemizedlist>
      </para>
      <section>
        <title>Simple scan</title>
        <para>
          Examples may be in order here. Let us start with a simple case, the sample is 
          rotated around its rotation axis and data is collected in a single point 
          detector. See figure <link xlink:href="#table.SimpleScan">Simple Scan </link> for an overview.
          Then we have:
          <itemizedlist>
            <listitem>
              <para>
                A dataset at <code>NXentry/NXinstrument/NXdetector/data</code> 
                of length <code>NP</code> containing 
                the count data.
              </para>
            </listitem>
            <listitem>
              <para>
                A dataset at <code>NXentry/NXsample/rotation_angle</code> 
                of length <code>NP</code> containing 
                the positions of <code>rotation_angle</code> at the various steps of the scan.
              </para>
            </listitem>
            <listitem>
              <para>
                <code>NXdata</code> contains links to 
                <code>NXentry/NXinstrument/NXdetector/data</code> and 
                <code>NXentry/NXsample/rotation_angle</code>.
              </para>
            </listitem>
            <listitem>
              <para>
                All other data fields have their normal dimensions.
              </para>
            </listitem>
          </itemizedlist>
        </para>
        <figure xml:id="table.SimpleScan">
          <title>NeXus Simple Scan Example</title>
          <programlisting linenumbering="numbered"
            ><xi:include href="examples/simplescan.txt" parse="text"
            /></programlisting>
        </figure>
       </section>
        
      <section>
        <title>Simple scan with area detector</title>
        <para>
        The next example is the same scan but with an area detector with <code>xsize</code> 
        times <code>ysize</code> pixels. The only thing which changes is that
        <code>/NXentry/NXinstrument/NXdetector/data</code> will have the dimensions 
      <code>NP, xsize, ysize</code>. See figure <link xlink:href="#fig.SimpleScanArea">Simple Scan with Area Detector
         </link> for an overview. 
     </para>
     
     <figure xml:id="fig.SimpleScanArea">
       <title>NeXus Simple Scan Example with Area Detector</title>
       <programlisting linenumbering="numbered"
          ><xi:include href="examples/simplescanarea.txt" parse="text"
          /></programlisting>
     </figure>
        </section>
        
       
      <section>
        <title>Complex <emphasis>hkl</emphasis> scan</title>
        <para>
          The next example involves a complex movement along an axis in reciprocal 
          space which requires mutiple motors of a four circle diffractometer to be 
          varied during the scan. We then have:
        <itemizedlist>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXinstrument/NXdetector/data</code> of length 
              <code>NP</code> containing 
              the count data.
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXinstrument/NXdetector/polar_angle</code> of length 
              <code>NP</code> containing 
              the positions of the detector's polar_angle at the various steps 
              of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/rotation_angle</code> of length 
              <code>NP</code> containing 
              the positions of <code>rotation_angle</code> at the various steps of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/chi</code> of length <code>NP</code> containing 
              the positions of chi at the various steps of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/phi</code> of length <code>NP</code> containing 
              the positions of phi at the various steps of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/h</code> of length <code>NP</code> containing 
              the positions of the reciprocal coordinate <code>h</code> at the 
              various steps of the scan.
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/k</code> of length <code>NP</code> containing 
              the positions of the reciprocal coordinate <code>k</code> at the 
              various steps of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              A dataset at <code>NXentry/NXsample/l</code> of length <code>NP</code> containing 
              the positions of the reciprocal coordinate <code>l</code> at the 
              various steps of the scan.  
            </para>
          </listitem>
          <listitem>
            <para>
              <code>NXdata</code>  contains links to 
              <code>NXentry/NXinstrument/NXdetector/data</code>, 
              <code>NXentry/NXinstrument/NXdetector/polar_angle</code>, 
              <code>NXentry/NXsample/rotation_angle</code>, 
              <code>NXentry/NXsample/chi</code>, 
              <code>NXentry/NXsample/phi</code>, 
              <code>NXentry/NXsample/h</code>, 
              <code>NXentry/NXsample/k</code> and 
              <code>NXentry/NXsample/l </code>
              The datasets in <code>NXdata</code> must have the 
              appropriate attributes as described in the axis location section.
            </para>
          </listitem>
          <listitem>
            <para>
              All other data fields have their normal dimensions.
            </para>
          </listitem>
        </itemizedlist>
      </para>
      <figure xml:id="table.ComplexScan">
        <title>NeXus Complex <emphasis>hkl</emphasis> Scan</title>
        <programlisting linenumbering="numbered"
          ><xi:include href="examples/complex-hkl-scan.txt" parse="text"
        /></programlisting>
      </figure>
      </section>
      <section>
        <title>Multi-parameter scan: XAS</title>
        <para>
          Data can be stored almost anywhere in the NeXus tree.  While the previous examples
          showed data arrays in either <literal>NXdetector</literal> or <literal>NXsample</literal>,
          this example demonstrates that data can be stored in other places.  Links are used 
          to reference the data.
        </para>
        <para>
          The example is for X-ray Absorption Spectroscopy (XAS) data where the monochromator
          energy is step-scanned and counts are read back from detectors before (<literal>I0</literal>)
          and after (<literal>I</literal>) the sample.  These energy scans are repeated at a sequence
          of sample temperatures to map out, for example, a phase transition.  While it is customary 
          in XAS to plot <emphasis>log(I0/I)</emphasis>, we show them separately here 
          in two different <literal>NXdata</literal>
          groups to demonstrate that such things are possible.
          Note that the length of the 1-D energy array is <literal>NE</literal> while
          the length of the 1-D temperature array is <literal>NT</literal>
        </para>
        <figure xml:id="table.XasScan">
          <title>NeXus Multi-parameter scan: XAS</title>
          <programlisting linenumbering="numbered"
          ><xi:include href="examples/xas.txt" parse="text"
        /></programlisting>
        </figure>
      </section>
  </section>
       
       
    <section xml:id="Rules-SpecialCases-Rastering">
      <title>Rastering</title>
      <para>
        Rastering is the process of making experiments at various locations in the 
        sample volume. Again, rasterisation experiments can be variable. Some people 
        even raster on spirals! Rasterisation experiments are treated the same way as 
        described above for scans. Just replace <code>NP</code> with 
        <code>P</code>, the number of raster points.
      </para>
      <para>
        Special rules apply if a rasterisation happens on a regular grid of size 
        <code>xraster, yraster</code>. Then the variables varied in the rasterisation will be 
        of dimensions <code>xraster, yraster</code> and the detector data of dimensions 
    		<code>xraster, yraster, (orginal dimensions)</code> 
    		of the detector. For example, an area detector of 
        size <code>xsize, ysize</code> then it is stored with
        dimensions <code>xraster, yraster, xsize, ysize</code>. 
      <warning>
        <para>
          Be warned: if you use the 2D rasterisation method with <code>xraster, yraster</code> you may end up with 
          invalid data if the scan is aborted prematurely. This cannot happen if the first method is used. 
        </para>
      </warning>
      </para>
      </section>

  <section>
    <title>NXcollection</title>
    <para>
      On demand from the community, NeXus introduced a more informal method of storing 
      information in a NeXus file.  This is the <literal>NXcollection</literal> class which can appear anywhere 
      underneath <literal>NXentry</literal>. <literal>NXcollection</literal> is a container for holding other data. 
      The foreseen use is to document collections of similar data which do not otherwise fit easily into the <code>NXinstrument</code> 
      or <code>NXsample</code> hierarchy, such as the intent to record <emphasis>all</emphasis> motor positions on a synchrotron beamline.
      Thus, <literal>NXcollection</literal> serves as a quick point of access to data for an instrument scientist or another expert.
      An example usage case is documented in  figure <link xlink:href="#table.SimpleScanArea">NXcollection example
         </link>. </para>
    <figure xml:id="table.NXcollection">
      <title>NXcollection Example</title>
      <!-- TODO This use case could show how components in NXinstrument are linked to members of an NXcollection -->
      <programlisting linenumbering="numbered"
        ><xi:include href="examples/nxcollection.txt" parse="text"
        /></programlisting>
    </figure>     
    <para>
      <!-- 2010-12-23,PRJ:  This point can be relaxed when considering the conversion of SPEC data files into NeXus.
        It is understood that NXcollection will never be part of a NeXus application definition for now. 
      This may change in a later stage when a common usage pattern for NXcollection emerges.  -->
      Data in <literal>NXcollection</literal> has to be augmented by a <literal>NXsubentry</literal> 
      or <literal>NXentry</literal> adhering to an application definition. 
    </para>
  </section>
     
    </section>
 </section>   
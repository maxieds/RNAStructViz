                                             
By providing immediate visual and statistical comparison of RNA secondary structures, and allowing users to quickly change the sets of structures displayed, RNAStructViz supports more interactive comparison of RNA secondary structures than was previously available. 

![RNAStructViz](https://raw.github.com/gtfold/RNAStructViz/master/img/multiple_windows.png)

###Loading Structures
RNAStructViz accepts secondary structures in both .ct and .bpseq formats. As structures are loaded, they are automatically grouped into folders according to their underlying RNA sequence. Structures based on the same underlying sequence are grouped together, with the sequence parser ignoring case (upper vs. lower) and treating 'T' and 'U' as equivalent.

![Main Menu](https://raw.github.com/gtfold/RNAStructViz/master/img/main_menu.png)

###Managing Structures and Windows
The main menu, lists the automatically generated sequence-based folders. In this  which can be reordered or removed using the adjacent buttons. Selecting a folder displays its contents, and brings up buttons that allow the user to to open Diagram and Statistics windows for that folder. Users can open as many  Diagram and Statistics windows for each sequence-folder as they wish. This feature allows users to compare multiple sets of structures simultaneously.

![S.Cerevisiae 5S Diagram](https://raw.github.com/gtfold/RNAStructViz/master/img/cerevisiae_diagram.png)

![C.Elegans 16S Diagram](https://raw.github.com/gtfold/RNAStructViz/master/img/elegans_diagram.png)

Users can compare up to three structures on the same diagram. RNAStructViz displays RNA secondary structures using a circular arcplots, where a circular backbone represents the RNA molecule’s underlying sequence, and an arc connects two positions in this backbone if the corresponding bases form a base-pair in the given secondary structure. Arcs are color-coded to represent which structures contain the base-pair.

- Base-pairs present in only the first structure will be colored red; the second and third will be colored green and blue, respectively.
- Base-pairs present in only two structures are colored using the RGB combination of those structure colors, e.g. yellow for the first and second structures.
- Base-pairs present in all structures are colored black.

![S.Cerevisiae 5S Chart](https://raw.github.com/gtfold/RNAStructViz/master/img/cerevisiae_chart.png)

![C.Elegans 16S Chart](https://raw.github.com/gtfold/RNAStructViz/master/img/elegans_charts.png)

Statistics windows allow direct quantitative comparison of structures against a selected reference structure. After the user selects structures for comparison and reference, RNAStructViz calculates and displays the following statistics for each structure comparing it to the reference:

- True positive (TP): base-pairs present in both the reference and selected structures
- False negatives (FN): base-pairs present in the reference but not the selected structure
- False positives (FP): base-pairs present in the selected but not the reference structure
- Sensitivity: TP / (TP + FN)
- Selectivity: TP / (TP + FP - c) where c is the number of base-pairs in the selected structure that are not present but do not otherwise conflict with base-pairs in the reference
- Positive predictive value (PPV): TP / (TP + FP)

Histograms for these statistics are displayed in tabs inside the statistics window. Another tab displays a Receiver Operating Characteristic (ROC) plot which plots selectivity versus sensitivity for each structure. The final tab displays all the calculated statistics in a table that can be exported to a comma-delimited file.

### Authors and Contributors
This software was developed by the Discrete Mathematical Biology lab group at Georgia Tech, run by Dr. Christine Heitsch. We thank Stephen Chenney for the initial design and prototype, and Anna Morrow for significant contributions to the diagram window functionality. Chris Mize (@themize) brought it to it's current incarnation with feedback from and the members of Discrete Mathematical Biology Group regarding features and the user interface.
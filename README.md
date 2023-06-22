CLASS_StepDR: CLASS with "Stepped Dark Radiation" {#mainpage}
==============================================

Author: Zilu Zhou

Description
--------------
Adapted from the CLASS (Cosmic Linear Anisotropy Solving System) code developed by Julien Lesgourgues, Thomas Tram, Nils Schoeneberg and collaborators.

On top of full base CLASS species and utilities (as of interacting dark matter commit on 03/29/2022), this modified version of CLASS contains a new interacting dark radiation specie that contains a "step" in the history of its perturbation growth. The current implementation of one such specie follows the "Wess-Zumino Dark Radation" (WZDR) model considered in Aloni et al. (2022), but more stepped fluid species will be added.

All modifications to CLASS source files are enclosed in comments that read "Stepped fluid modification". All functions and parameters related to the stepped dark radiations have names containing "stepped_fld". The major implementations are contained in the background and perturbation modules.

Running the code
--------------
Installation, compiling and setting up the Python wrapper all work identically to base CLASS. See the official page

https://github.com/lesgourg/class_public

for detailed guide on installation. If you run into trouble installing/running the code but can otherwise run base CLASS, please open an issue.

A sample input containing the stepped fluid can be found in inputs/WZDR.ini, and contains some information on how to pass in stepped fluid parameters.

Using the code
--------------

You can use CLASS freely, provided that in your publications, you cite
at least the paper `CLASS II: Approximation schemes <http://arxiv.org/abs/1104.2933>`. Feel free to cite more CLASS papers!


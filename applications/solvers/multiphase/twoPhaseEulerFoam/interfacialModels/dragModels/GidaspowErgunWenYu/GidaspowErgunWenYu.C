/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "GidaspowErgunWenYu.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(GidaspowErgunWenYu, 0);

    addToRunTimeSelectionTable
    (
        dragModel,
        GidaspowErgunWenYu,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::GidaspowErgunWenYu::GidaspowErgunWenYu
(
    const dictionary& interfaceDict,
    const volScalarField& alpha1,
    const phaseModel& phase1,
    const phaseModel& phase2
)
:
    dragModel(interfaceDict, alpha1, phase1, phase2)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::GidaspowErgunWenYu::~GidaspowErgunWenYu()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField> Foam::GidaspowErgunWenYu::K
(
    const volScalarField& Ur
) const
{
    volScalarField alpha2(max(scalar(1) - alpha1_, scalar(1.0e-6)));

    volScalarField bp(pow(alpha2, -2.65));
    volScalarField Re(max(Ur*phase1_.d()/phase2_.nu(), scalar(1.0e-3)));

    volScalarField Cds
    (
        neg(Re - 1000)*(24.0*(1.0 + 0.15*pow(Re, 0.687))/Re)
      + pos(Re - 1000)*0.44
    );

    // Wen and Yu (1966)
    return
    (
        pos(alpha2 - 0.8)
       *(0.75*Cds*phase2_.rho()*Ur*bp/phase1_.d())
      + neg(alpha2 - 0.8)
       *(
           150.0*alpha1_*phase2_.nu()*phase2_.rho()/(sqr(alpha2*phase1_.d()))
         + 1.75*phase2_.rho()*Ur/(alpha2*phase1_.d())
        )
    );
}


// ************************************************************************* //

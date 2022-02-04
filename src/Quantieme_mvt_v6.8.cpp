/*
v1.0 (2020/08/13) : on met un triangle sur le PrixTrade à chaque fois qu'on arrive dessus après un mvt de >1%.
v1.1 (2020/09/03) : on affiche tous les mvts et leurs amplitudes.
v1.2 (2020/09/03) : le second (ou troisième) mvt est RAZ si on s'écarte de NiveauTrade de plus de deux fois de la valeur AmplitudeMaxDesMouvements.
v1.3 (2020/09/06) : prise en main de la Dynamic Memory Allocations Within Study Instance.
v1.4 (2020/09/06) : prise en main de vector.
v1.5 (2020/09/07) : Dynamic Memory Allocations Within Study Instance avec un vector.
v1.6 (2020/09/07) : ajout des extrema des mvts sur le graphe.
v1.7 (2020/09/07) : pour chaque mvt qui vient de se terminer, on itère sur les mvts précédents tant que le mouvement actuel est plus important que les précédents. Plus important signifie :
									- pour un mvt de même sens : l'amplitude du mvt actuel est au moins deux fois supérieure à l'amplitude du mvt précédent.
									- pour un mvt de sens inverse : l'amplitude du mvt actuel est supérieure à l'amplitude du mvt précédent.
									ALORS : le décompte du mouvement (avant incrémentation) est pris égal au décompte de ce mvt plus important.
v1.8 (2020/09/08) :  débuggage : tous les TypeMouvementPrecedent sont supprimés. TypeMouvement est calculé simplement (vaut 1 s'il a la même parité que le mvt initial, -1 sinon).
v2.0 (2020/09/08) : ajouter les conditions selon la nature des niveaux (psys / J) identiques à "Amplitude Mvt".
v2.1 (2020/09/09) : ajout de IndexFinPeriode. Debuggage sur IndexDebutPeriode de niveau psy.
v2.2 (2020/09/09) : ajouter les conditions selon la nature des niveaux (H / M) identiques à "Amplitude Mvt".
v2.3 (2020/09/09) : asservir la study à AMR.
v2.4 (2020/09/09) : modif de IncrementationComptageMouvements.
v2.5 (2020/09/09) : correction de bug.
v3.0 (2020/09/10) : ajout du bouton 22 qui affiche le numéro de mvt.
v3.1 (2020/09/10) : résolution bug ("Mvt 0") au mauvais endroit. Durée de calcul du DLL à chaque appel : 10-20 microsecondes.
v3.2 (2020/09/20) : correction de bug : le premier mouvement n'était pas identifié lorsque AmplitudeActuelle_pdm < MouvementRelatif_Seuil.
v3.3 (2020/09/21) : introduction de Interligne pour les tracés.
v3.4 (2020/09/21) : fix d'un bug lorsque la première bougie de la période inclut Niveau.
v3.5 (2020/09/21) : fix d'un bug lié à OpenGap (introduction de bool_InsideOpenGap).
v3.6 (2020/09/22) : fix d'un bug lié aux PP 1h/4h.
v3.7 (2020/09/22) : fix d'un bug lié à OpenGap.
v4.0 (2020/09/30) : dans StructureMouvement, on ajoute le membre IndexCrossing.
v4.1 (2020/09/30) : détermination de NumeroMouvement_EnCours pour le mouvement succédant à HoraireRAZ.
v4.2 (2020/09/30) : modif de SetButton22_NumeroMouvement.
v4.3 (2020/09/30) : fix d'un bug lié aux niveaux psys.
v4.4 (2020/09/30) : fix d'un bug lié à SetButton22_NumeroMouvement.
v5.0 (2020/10/01) : le DLL "Amplitude Mvt" est incorporé dans le DLL "Quantieme Mvt".
v5.1 (2020/10/02) : finalisation de v5.0.
v5.2 (2020/10/02) : on adapte Epsilon.
v5.3 (2020/10/02) : poursuite de la "base de données" de CalculDuBonus.
v5.4 (2020/10/05) : homogénéisation des arrondis (graphe vs boutons).
v5.5 (2020/10/05) : ajout du bouton 19 réservé à l'affichage des bonus.
v5.6 (2020/10/06) : CalculDuBonus est modifié pour inclure le Bonus_PlanA.
v5.7 (2020/10/06) : les variables persistantes SCString qui stockent les affichages des boutons 18/19/22 sont gérées uniquement par la study du chart 15.
v5.8 (2020/10/08) : fix bug (niveau effleuré non dépassé).
v6.0 (2020/10/08) : DebutPeriode de PP 1h et de PP 4h est mis en oeuvre.
v6.1 (2020/10/09) : le button 18 fait apparaitre le nb de Full (au lieu de Amplitude en points).
v6.2 (2020/10/15) : le calcul de QuantiemeMvt pour un PP 1h ou 4h n'est autorisé que dans la fenêtre #15 (UT 20s) et si cette fenêtre est visible (note : cette restriction peut être levée si les charts se resynchronisent lorsqu'ils ne sont pas affichés).
v6.3 (2020/10/20) : fix d'un bug lié à OpenGap.
v6.4 (2020/10/21) : fix d'un bug lié à "> Fin Periode".
v6.5 (2020/10/23) : gestion de l'open gap pour les niveaux psys.
v6.6 (2021/01/05) : fix bug lors d'un open gap pour les niveaux psys.
v6.7 (2021/01/11) : on ajout un input qui contient Amplitude_pdm, qui sera utilisé en lecture par AMR.
v6.8 (2021/01/14) : résolution de l'autorisation du PP 1h d'être calculé même si le chart #15 n'est pas visible.








Améliorations à apporter :
- utiliser un mvt seuil différent selon qu'on soit sur le chart 15 ou 19.


*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Quantieme Mvt")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigne : s_UseTool  // MaLigne crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		MaLigne(float niveau, int epaisseur, COLORREF rgb, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			AddMethod = UTAM_ADD_OR_ADJUST ;		
			DrawingType = DRAWING_HORIZONTALLINE;
			LineStyle = /* LINESTYLE_SOLID */ LINESTYLE_DASH;
			BeginValue = niveau;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = Graphregion ;   // 0 par défaut		
		} 
	};
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLignePointilleMarron (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		//Création d'instance de MaLigne et appel de sc.UseTool
		MaLigne objetMaLigne(monNiveau, monEpaisseur, maCouleur, ptrsc->GraphRegion);
		ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteAjusteDroite_UTAMAOA : s_UseTool   
{											
	TexteAjusteDroite_UTAMAOA (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = tailleTexte ;
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_RIGHT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		FontBackColor = RGB(255,255,255) ;
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawTextAjusteDroite_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	TexteAjusteDroite_UTAMAOA objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetTexte); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker_UTAMAOA : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion/* , int Line_Number */) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		// LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;		// marqueur = MARKER_POINT, MARKER_DASH, MARKER_SQUARE, MARKER_STAR, MARKER_PLUS, MARKER_X, MARKER_ARROWUP, MARKER_ARROWDOWN, MARKER_ARROWRIGHT, MARKER_ARROWLEFT, MARKER_TRIANGLEUP, MARKER_TRIANGLEDOWN, MARKER_TRIANGLERIGHT, MARKER_TRIANGLELEFT, MARKER_DIAMOND
		BeginIndex = index ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc/* , int Line_Number */)  
{
	//Création d'instance de Marker_UTAMAOA et appel de sc.UseTool
	Marker_UTAMAOA objetMarker(marqueur, index, prix, tailleObjet, couleur, ptrsc->GraphRegion/* , Line_Number */);
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SegmentPointille_UTAMAOA : s_UseTool  
{
	SegmentPointille_UTAMAOA (int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear() ; 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		LineNumber = Line_Number ;		
		DrawingType = DRAWING_LINE ;
		LineStyle = LINESTYLE_DASH ;    // LineStyle = LINESTYLE_DOT ne permet pas de tracer avec LineWidth > 1. Il faut alors utiliser LINESTYLE_DASH pour tracer avec une épaisseur > 1.
		BeginIndex = index1 ;
		BeginValue = prix1 ;
		EndIndex = index2 ;
		EndValue = prix2 ;
		LineWidth = epaisseur;
		Color = couleur; 
		Region = graphRegion ;   // 0 par défaut
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSegmentPointille_UTAMAOA (int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	SegmentPointille_UTAMAOA objetSegmentPointille(index1, prix1, index2, prix2, epaisseur, couleur, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetSegmentPointille); // c'est sc.UseTool qui est appelé
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte_UTAMAOA : s_UseTool   
{											
	Texte_UTAMAOA (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, int Alignement /* , int Line_Number */) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		// LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = tailleTexte ;
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = Alignement ;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		FontBackColor = RGB(255,255,255) ;
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, int Alignement, s_sc* ptrsc/* , int Line_Number */)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	Texte_UTAMAOA objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->GraphRegion, Alignement /* , Line_Number */);
	ptrsc->UseTool(objetTexte); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int IncrementationComptageMouvements(int NumeroMouvementPrecedent, int TypeMouvementActuel, s_sc* ptrsc)			// retourne NumeroMouvement à partir de NumeroMouvementPrecedent
{
	int NumeroMouvement ;
	int& TypeMouvementInitial = ptrsc->GetPersistentIntFast(0); 
	
	if ( NumeroMouvementPrecedent == 0 )
	{ 
		NumeroMouvement = 1 ;  
		TypeMouvementInitial = TypeMouvementActuel ;  
	}
	else
	{
		// on fait une première icrémentation
		NumeroMouvement = NumeroMouvementPrecedent + 1 ;
		
		// NumeroMouvement doit avoir une parité exprimée par rapport à TypeMouvementInitial
		if ( TypeMouvementActuel == TypeMouvementInitial )  						// même parité i.e. NumeroMouvement doit être impair
		{
			if ( NumeroMouvement %2 == 0)  NumeroMouvement ++ ;			// NumeroMouvement était pair => on l'incrémente
		}
		else if ( TypeMouvementActuel != TypeMouvementInitial )  				// parité différente i.e. NumeroMouvement doit être pair
		{
			if ( NumeroMouvement %2 == 1)  NumeroMouvement ++ ;			// NumeroMouvement était impair => on l'incrémente
		}
	}
	
	return NumeroMouvement ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetButton22_NumeroMouvement (SCString Texte, int NumeroMouvement_EnCours, s_sc* ptrsc)			// "Num Mvt = %d"
{
	SCInputRef 	NatureNiveau_Input = ptrsc->Input[3];  
	int NatureNiveau = NatureNiveau_Input.GetInt() ;
	
	if ( ( ptrsc->ChartNumber == 15 and NatureNiveau != 0 and NatureNiveau != 5)  or ( ptrsc->ChartNumber == 19 and ( NatureNiveau == 0 or NatureNiveau == 5 ) ) )    	// Niveaux psys et M avec le chart 19 
	{ 
		// Button 22
		SCString Texte_Button22 ;
		if ( Texte == "" ) { Texte_Button22.Format("Num Mvt = %d", NumeroMouvement_EnCours) ;  }
		else Texte_Button22 = Texte ;
		
		// TextePrecedent_Button22
		int& StudyID_QuantiemeMvt_Chart15 = ptrsc->GetPersistentIntFast(14); 
		SCString TextePrecedent_Button22 = ptrsc->GetPersistentSCStringFromChartStudy(15, StudyID_QuantiemeMvt_Chart15, 0) ;			// GetPersistentSCStringFromChartStudy(int ChartNumber, int StudyID, int Key);
		
		// Modif de TextePrecedent_Button22
		if ( Texte_Button22 != TextePrecedent_Button22 )
		{
			ptrsc->SetCustomStudyControlBarButtonText(22, Texte_Button22) ; 		
			ptrsc->SetPersistentSCStringForChartStudy(15, StudyID_QuantiemeMvt_Chart15, 0, Texte_Button22) ;			// void SetPersistentSCStringForChartStudy(int ChartNumber, int StudyID, int Key, SCString Value);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Struct_Ligne_Bonus			// Structure pour une entrée de data de Amplitude/Bonus
{
	// variables membres :
	float Amplitude_pdm_ref ;
	float Bonus_ref ;
	
	/* // Constructeur par défaut
	Struct_Ligne_Bonus()
	{
		Amplitude_pdm_ref = -88 ;
		Bonus_ref = -88 ;
	} */
	
	// Constructeur
	Struct_Ligne_Bonus(float a, float b)
	{
		Amplitude_pdm_ref = a ;
		Bonus_ref = b ;
	}
} ;	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Interpolation_Logarithmique (std::vector<Struct_Ligne_Bonus> & Vector_Bonus, float const & Amplitude_pdm)			// Amplitude_pdm est normé à 10000
{
	// on cherche les couples (Amplitude_pdm_ref_1, Bonus_ref_1) et (Amplitude_pdm_ref_2, Bonus_ref_2) du tableau "Réactivations Excel" dont les valeurs encadrent Amplitude_pdm
	float Amplitude_pdm_ref_1 = -1 ;
	float Amplitude_pdm_ref_2 = 999999 ;
	float Bonus_ref_1 = 999;
	float Bonus_ref_2 = 999;
	
	for (auto it : Vector_Bonus )			// les contenus du vecteur Vector_Bonus sont parcourus entre begin et back à l'aide d'un itérateur, pour trouver la borne sup ( Amplitude_pdm_ref_2 )
	{
		if ( Amplitude_pdm  <= it.Amplitude_pdm_ref ) 
		{
			Amplitude_pdm_ref_2 = it.Amplitude_pdm_ref ;
			Bonus_ref_2 = it.Bonus_ref ;
			break ;
		}
	}

	for (std::vector<Struct_Ligne_Bonus>::reverse_iterator  rit = Vector_Bonus.rbegin(); rit != Vector_Bonus.rend(); ++rit) 			// les contenus du vecteur Vector_Bonus sont parcourus entre back et begin à l'aide d'un reverse iterator rit, pour trouver la borne inf ( Amplitude_pdm_ref_1 )
	{
		if ( rit->Amplitude_pdm_ref <= Amplitude_pdm ) 
		{
			Amplitude_pdm_ref_1 = rit->Amplitude_pdm_ref ;
			Bonus_ref_1 = rit->Bonus_ref ;
			break ;
		}
	}

	
	if ( Amplitude_pdm_ref_1 == -1 )		// Amplitude_pdm < borne inf
	{
		return -1 ;
	}
	else	if ( Amplitude_pdm_ref_2 == 999999 )		// Amplitude_pdm > borne sup
	{
		return Vector_Bonus.back().Bonus_ref ;
	}
	else
	{
		// Interpolation du Bonus selon le modèle : Bonus = C * log ( Amplitude_pdm ) + D		// note : "log" est le log népérien
		float C = ( Bonus_ref_2 - Bonus_ref_1 ) / log ( Amplitude_pdm_ref_2 / Amplitude_pdm_ref_1 ) ;
		float D = Bonus_ref_1 - C * log ( Amplitude_pdm_ref_1 ) ;
		return C * log ( Amplitude_pdm ) + D ;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalculDuBonus(int NatureNiveau, int SousNatureNiveau, int NumeroMouvement, float Amplitude_pdm, float RatioAmplitude_PlanA, s_sc* ptrsc, float & Bonus_PlanA, float & Bonus_PlanB)			// Base de données
{
	// initialisation
	Bonus_PlanA = -1 ;		
	Bonus_PlanB = -1 ;		
	
	// vector pour niveaux psys
	std::vector<Struct_Ligne_Bonus> Vector_Bonus ;
	
	// MultiplicateurFull4Mid4
	float MultiplicateurFull4Mid4 = 1 ;
	if ( SousNatureNiveau == 14  or  SousNatureNiveau == 24 )  MultiplicateurFull4Mid4 = 0.75 ;
	
	//// Base de données des NatureNiveau, SousNatureNiveau et NumeroMouvement
	// Niveaux psys  
	if ( NatureNiveau == 0 )			
	{
		if (SousNatureNiveau == 1000 or SousNatureNiveau == 500 or SousNatureNiveau == 250 or SousNatureNiveau == 100 ) 					// SousNatureNiveau = 100 (centaines), 250, 500 et 1000
		{
			Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		18) ) ;			// Struct_Ligne_Bonus correspond à un couple (Amplitude_pdm_ref, Bonus_ref)
			Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		26) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		31) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		33) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		35) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		40) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		48) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		56) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	59) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	63) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	65) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	72) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	74) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(2000, 	75) ) ;
			
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			// SCString message ;  message.Format(" Niveau psy 100/250/500/1000 : Bonus_PlanB = %.4f", Bonus_PlanB) ;  ptrsc->AddMessageToLog(message, 0) ;
		}
		else if ( SousNatureNiveau == 333 )	
		{
			Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		13) ) ;			
			Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		21) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		27) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		30) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		34) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		38) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		43) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	47) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(220, 	49) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(440, 	53) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(880, 	58) ) ;
			
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
		}
		else if ( SousNatureNiveau == 125 )	
		{
			Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		14) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		22) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		31) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	38) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(240, 	49) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	60) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	64) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(2000, 	65) ) ;
			
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
		}
		else if ( SousNatureNiveau == 50 )	
		{
			Vector_Bonus.push_back( Struct_Ligne_Bonus(9, 		12) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		19) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		20) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(38,		23) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(50, 		26) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		30) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(100, 	33) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	36) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	38) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	39) ) ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(2000, 	40) ) ;
			
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			SCString message ;  message.Format(" Niveau 50 : Bonus_PlanB = %.4f", Bonus_PlanB) ;  ptrsc->AddMessageToLog(message, 0) ;
		}
	}
	
	// Niveaux 1h
	else if ( NatureNiveau == 1 )		
	{		
		if ( NumeroMouvement == 1 )
		{
			// Plan A : pour simplifier les fonctions d'affichage, on l'appelle PlanB
			Vector_Bonus.clear() ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(0.01, 		26*MultiplicateurFull4Mid4) ) ;		// premier full touché à h+0
			Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		26*MultiplicateurFull4Mid4) ) ;		// Premier Full touché après un Mid
			Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			30*MultiplicateurFull4Mid4) ) ;		// Second Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			32*MultiplicateurFull4Mid4) ) ;		// Troisième Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			34*MultiplicateurFull4Mid4) ) ;		// 4ème Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(4, 			36*MultiplicateurFull4Mid4) ) ;		// 5ème Full d'un mvt
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			// SCString message ;  message.Format(" PP 1h : RatioAmplitude_PlanA = %.4f, Bonus_PlanB = %.4f", RatioAmplitude_PlanA, Bonus_PlanB) ;  ptrsc->AddMessageToLog(message, 0) ;
		}
	}
	
	// Niveaux 4h
	else if ( NatureNiveau == 2 )			
	{		
		if ( NumeroMouvement == 1 )
		{
			// Plan A : pour simplifier les fonctions d'affichage, on l'appelle PlanB
			Vector_Bonus.clear() ;
			Vector_Bonus.push_back( Struct_Ligne_Bonus(0.01, 		26*MultiplicateurFull4Mid4) ) ;		// premier full touché à h+0
			Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		30*MultiplicateurFull4Mid4) ) ;		// Premier Full touché après un Mid
			Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			32*MultiplicateurFull4Mid4) ) ;		// Second Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			38*MultiplicateurFull4Mid4) ) ;		// Troisième Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			42*MultiplicateurFull4Mid4) ) ;		// 4ème Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(4, 			44*MultiplicateurFull4Mid4) ) ;		// 5ème Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(5, 			46*MultiplicateurFull4Mid4) ) ;		// 6ème Full d'un mvt
			Vector_Bonus.push_back( Struct_Ligne_Bonus(6, 			48*MultiplicateurFull4Mid4) ) ;		// 7ème Full d'un mvt
			Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			// SCString message ;  message.Format(" PP 4h : RatioAmplitude_PlanA = %.4f, Bonus_PlanB = %.4f", RatioAmplitude_PlanA, Bonus_PlanB) ;  ptrsc->AddMessageToLog(message, 0) ;
		}
	}
	
	// Niveaux J
	else if ( NatureNiveau == 3 )			
	{	
		if ( SousNatureNiveau == 1  or  SousNatureNiveau == 14 )		// Full
		{
			
			if ( NumeroMouvement == 1 )
			{
				// Plan B
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		32	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		37	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		41	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		47	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		48	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		49	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		50	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		51	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		53	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	54	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	56	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	59	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	61	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	62	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		45	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		47	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		54	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		56	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			58	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			59	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			61	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				
				// Plan B
				Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		12	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		28	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		44	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		56	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		58	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		59	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		62	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	72	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	74	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		64	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.9 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		12	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		28	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		44	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		56	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		58	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		59	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		62	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	72	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	74	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		64	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;	
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		12	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		28	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		44	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		56	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		58	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		59	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		62	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	72	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	74	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		64	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;	
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		12	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		28	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		44	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		56	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		58	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		59	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		62	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	72	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	74	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		64	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		
			}
			else if ( NumeroMouvement == 6 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(7, 		12	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		28	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		44	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30,		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		56	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		58	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		59	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		62	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180, 	68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	72	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	74	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/4, 		64	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;	
			}
		}
		else if ( SousNatureNiveau == 2  or  SousNatureNiveau == 24 )		// Mid
		{
			if ( NumeroMouvement == 1 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		14	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		26	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		33	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27,		40	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		43	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		46	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		49	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		51	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	54	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	55	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	57	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		33	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		44	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		57	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0, 		64	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		65	*MultiplicateurFull4Mid4) ) ;		// 2nd Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		66	*MultiplicateurFull4Mid4) ) ;		// 3ème Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		14	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		26	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		33	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27,		40	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		43	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		46	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		48	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	51	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	52	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		16	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		33	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		54	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0, 		55	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		56	*MultiplicateurFull4Mid4) ) ;		// 2nd Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		57	*MultiplicateurFull4Mid4) ) ;		// 3è Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;	
			}
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		8		*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		12	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		15	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		19	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27,		24	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		29	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		31	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		32	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		34	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	35	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	36	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		10	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		15	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		38	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0, 		40	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		41	*MultiplicateurFull4Mid4) ) ;		// 2nd Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		42	*MultiplicateurFull4Mid4) ) ;		// 3è Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;		
			}
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		8		*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		12	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		15	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		19	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27,		24	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		29	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		31	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		32	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		34	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	35	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	36	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		10	*MultiplicateurFull4Mid4) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		15	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		38	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0, 		40	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		41	*MultiplicateurFull4Mid4) ) ;		// 2nd Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;	
			}
		}
		else if ( SousNatureNiveau == 6 )			// PH JP ou PB JP
		{  
			if ( NumeroMouvement == 1 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8, 		60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		68) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		72) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		73) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		85) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		89) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		91) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	95) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	103) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	112) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	115) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8, 		11) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		15) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		21) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		32) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		46) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		49) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		51) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		52) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	75) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		9) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		12) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		23) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		36) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		38) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		9) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		12) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		23) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		36) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		38) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
		else if ( SousNatureNiveau == 7 )			// Close
		{   
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		10*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		20*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		27*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27, 		33*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		40*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		43*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65,		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	45*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	46*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan B
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.06, 		30	*Multiplicateur_NumeroMvt) ) ;		// pour Close Plan B, les valeurs sont fournies en pc TR JP
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.12, 		40	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		50	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		55	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			62	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			65	*Multiplicateur_NumeroMvt) ) ;		
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA) ;	
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		10*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		20*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		27*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27, 		33*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		40*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		43*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65,		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	45*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	46*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan B
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.06, 		15	*Multiplicateur_NumeroMvt) ) ;		// pour Close Plan B, les valeurs sont fournies en pc TR JP
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.12, 		25	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		42	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA) ;	
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		10*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		20*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		27*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27, 		33*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		40*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		43*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65,		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	45*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	46*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan B
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.06, 		15	*Multiplicateur_NumeroMvt) ) ;		// pour Close Plan B, les valeurs sont fournies en pc TR JP
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.12, 		25	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		42	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA) ;	
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		10*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		20*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		27*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(27, 		33*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		40*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		43*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65,		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	45*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	46*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan B
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.06, 		15	*Multiplicateur_NumeroMvt) ) ;		// pour Close Plan B, les valeurs sont fournies en pc TR JP
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.12, 		25	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		42	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			50	*Multiplicateur_NumeroMvt) ) ;		
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA) ;	
			}
		}
		else if ( SousNatureNiveau == 8 )			// Gap
		{   
			if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8, 		80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		86) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		90) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		92) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		97) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		100) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		105) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		109) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		111) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	115) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	120) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	125) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(390, 	130) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8, 		10) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		15) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		48) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		51) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		52) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		16) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		27) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		38) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 5 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		16) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		27) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		38) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
		else if ( SousNatureNiveau == 9 )			// Demi Gap
		{   
			if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(21, 		55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(29, 		60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	85) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	90) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	95) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8, 		10) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		16) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(21, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(29, 		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	65) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
	}
	
	// Niveaux H
	else if ( NatureNiveau == 4 )			
	{	
		if ( SousNatureNiveau == 1  or  SousNatureNiveau == 14 )		// Full H
		{
			if ( NumeroMouvement == 1 )
			{
				// Plan B
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	52	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	60	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		65	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		77	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			111	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;

				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.5 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 6 )
			{
				float Multiplicateur_NumeroMvt = 0.45 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 7 )
			{
				float Multiplicateur_NumeroMvt = 0.4 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 8 )
			{
				float Multiplicateur_NumeroMvt = 0.35 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(13, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(18, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(22, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(34, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(45, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(76, 		75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(89, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(105, 	90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(120, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(160, 	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	109	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	111	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	112	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	20	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 2  or  SousNatureNiveau == 24 )		// Mid H
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 0.9 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	38	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	45	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		50	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		60	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		68	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			81	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		84	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		90	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		95	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	15	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	15	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	15	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	15	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 6 )
			{
				float Multiplicateur_NumeroMvt = 0.5 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		29	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		34	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		42	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		47	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		66	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		68	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	75	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250, 	88	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500, 	96	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000, 	100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	15	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		63	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 3 )			// Quart H
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		17*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		23*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		35*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		49*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	57*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	62*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	67*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	25*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	26*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	35*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		40*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			57*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			63*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			67*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		70*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		17*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		23*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		35*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		49*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	57*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	62*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	67*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	7*Multiplicateur_NumeroMvt) ) ;			// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	10*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	32*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		40*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			57*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			63*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			67*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		70*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		17*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		23*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		35*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		49*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	57*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	62*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	67*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	7*Multiplicateur_NumeroMvt) ) ;			// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	10*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	32*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		40*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			57*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			63*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			67*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		70*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		17*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		23*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		35*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(55, 		49*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	57*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	62*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	67*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	7*Multiplicateur_NumeroMvt) ) ;			// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	10*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	32*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		40*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		45*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			57*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			63*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			67*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		70*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 6 )			// PH HP ou PB HP
		{  
			if ( NumeroMouvement == 1 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		85) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		90) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		93) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	100) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	105) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	110) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	115) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	116) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		44) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		47) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	53) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	80) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		15) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		39) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		42) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	49) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		15) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		39) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		42) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	49) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
		else if ( SousNatureNiveau == 8 )			// Gap H
		{  
			if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		70+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		75+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		80+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		85+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		90+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		93+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	100+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	105+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	110+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	135) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	136) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		44) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		47) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	53) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	80) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 5 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		39) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		41) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
	}
	
	// Niveaux M
	else if ( NatureNiveau == 5 )			
	{	
		if ( SousNatureNiveau == 1  or  SousNatureNiveau == 14 )		// Full M
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	77	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		116	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			145	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			150	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 0.9 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement ==6 )
			{
				float Multiplicateur_NumeroMvt = 0.5 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 7 )
			{
				float Multiplicateur_NumeroMvt = 0.4 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 8 )
			{
				float Multiplicateur_NumeroMvt = 0.35 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 9 )
			{
				float Multiplicateur_NumeroMvt = 0.3 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 10 )
			{
				float Multiplicateur_NumeroMvt = 0.25 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(8,		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(11, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(14, 		35	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		40	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		45	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(23, 		50	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(26, 		55	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(30, 		60	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(37, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(49, 		70	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		80	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(70, 		90	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(80, 		100	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(97, 		110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(111, 	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	120	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(157,	125	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	130	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(205,	135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	138	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	139	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	140	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	141	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	23	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	38	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		67	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		95	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			126	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Second Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			131	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Trosième Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3, 			135	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième (ou plus) Full
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 2  or  SousNatureNiveau == 24 )		// Mid M
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	70	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	72	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		85	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		100	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		105	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			110	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		117	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		120	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(4.5, 		122	*MultiplicateurFull4Mid4) ) ;		// Cinquième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 6 )
			{
				float Multiplicateur_NumeroMvt = 0.5 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 7 )
			{
				float Multiplicateur_NumeroMvt = 0.4 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 8 )
			{
				float Multiplicateur_NumeroMvt = 0.35 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		25	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(20, 		30	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		36	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		41	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		46	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(41, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		57	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(64, 		65	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		73	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(110, 	79	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	82	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183,	93	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	101	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370,	108	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	110	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730,	113	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1500,	117	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	22	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	27	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8, 		52	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/4, 		85	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quart
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/2, 		92	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Full
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			105	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Mid voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Deuxième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Troisième Full voisin
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.5, 		115	*Multiplicateur_NumeroMvt	*MultiplicateurFull4Mid4) ) ;		// Quatrième Full voisin
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 3 )			// Quart M
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.9 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.8 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 5 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else if ( NumeroMouvement == 6 )
			{
				float Multiplicateur_NumeroMvt = 0.6 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		22*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		26*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		42*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		44*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	48*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260,	52*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520,	54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050,	56*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	34*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	36*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/16,	46*Multiplicateur_NumeroMvt) ) ;		// Eighth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3.0/16,	50*Multiplicateur_NumeroMvt) ) ;		// 3è siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.25, 		52*Multiplicateur_NumeroMvt) ) ;		// Mid voisin (1 quart)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(0.5, 		58*Multiplicateur_NumeroMvt) ) ;		// 1er quart voisin (2 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1,			60*Multiplicateur_NumeroMvt) ) ;		// 2nd quart voisin (4 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,			62*Multiplicateur_NumeroMvt) ) ;		// 3è quart voisin (6 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2, 			64*Multiplicateur_NumeroMvt) ) ;		// 4è quart voisin (8 quarts) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(3,	 		65*Multiplicateur_NumeroMvt) ) ;		// 6è quart voisin (12 quarts) 
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 4 )			// Eighth M
		{
			if ( NumeroMouvement == 1 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		34*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		41*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		50*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		53*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	59*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	66*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250,	72*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500,	76*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000,	80*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	30*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	35*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8,		42*Multiplicateur_NumeroMvt) ) ;		// Quart voisin (1 eighth)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/8, 		51*Multiplicateur_NumeroMvt) ) ;		// 1er Eighth voisin (2 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(4.0/8,		61*Multiplicateur_NumeroMvt) ) ;		// 2è Eighth voisin (4 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(6.0/8,		65*Multiplicateur_NumeroMvt) ) ;		// 3è Eighth voisin (6 eighths) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			70*Multiplicateur_NumeroMvt) ) ;		// 4è Eighth voisin (8 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,	 		75*Multiplicateur_NumeroMvt) ) ;		// 6è Eighth voisin (12 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2,	 		80*Multiplicateur_NumeroMvt) ) ;		// 8è Eighth voisin (16 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5,	 		85*Multiplicateur_NumeroMvt) ) ;		// 10è Eighth voisin (20 eighths)
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else  if ( NumeroMouvement == 2 )
			{
				float Multiplicateur_NumeroMvt = 1 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		34*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		41*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		50*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		53*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	59*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	66*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250,	72*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500,	76*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000,	80*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	10*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	19*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8,		30*Multiplicateur_NumeroMvt) ) ;		// Quart voisin (1 eighth)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/8, 		51*Multiplicateur_NumeroMvt) ) ;		// 1er Eighth voisin (2 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(4.0/8,		61*Multiplicateur_NumeroMvt) ) ;		// 2è Eighth voisin (4 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(6.0/8,		65*Multiplicateur_NumeroMvt) ) ;		// 3è Eighth voisin (6 eighths) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			70*Multiplicateur_NumeroMvt) ) ;		// 4è Eighth voisin (8 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,	 		75*Multiplicateur_NumeroMvt) ) ;		// 6è Eighth voisin (12 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2,	 		80*Multiplicateur_NumeroMvt) ) ;		// 8è Eighth voisin (16 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5,	 		85*Multiplicateur_NumeroMvt) ) ;		// 10è Eighth voisin (20 eighths)
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else  if ( NumeroMouvement == 3 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		34*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		41*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		50*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		53*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	59*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	66*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250,	72*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500,	76*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000,	80*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	10*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	19*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8,		30*Multiplicateur_NumeroMvt) ) ;		// Quart voisin (1 eighth)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/8, 		51*Multiplicateur_NumeroMvt) ) ;		// 1er Eighth voisin (2 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(4.0/8,		61*Multiplicateur_NumeroMvt) ) ;		// 2è Eighth voisin (4 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(6.0/8,		65*Multiplicateur_NumeroMvt) ) ;		// 3è Eighth voisin (6 eighths) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			70*Multiplicateur_NumeroMvt) ) ;		// 4è Eighth voisin (8 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,	 		75*Multiplicateur_NumeroMvt) ) ;		// 6è Eighth voisin (12 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2,	 		80*Multiplicateur_NumeroMvt) ) ;		// 8è Eighth voisin (16 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5,	 		85*Multiplicateur_NumeroMvt) ) ;		// 10è Eighth voisin (20 eighths)
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
			else  if ( NumeroMouvement == 4 )
			{
				float Multiplicateur_NumeroMvt = 0.7 ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		15*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(28, 		30*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33, 		34*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(39, 		37*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		41*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(52, 		46*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(59, 		50*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		53*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		54*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130,	59*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(180,	66*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(250,	72*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(500,	76*Multiplicateur_NumeroMvt) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1000,	80*Multiplicateur_NumeroMvt) ) ;
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
				
				// Plan A
				Vector_Bonus.clear() ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/32,	10*Multiplicateur_NumeroMvt) ) ;		// 1/32e
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/16,	19*Multiplicateur_NumeroMvt) ) ;		// siXteenth
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.0/8,		30*Multiplicateur_NumeroMvt) ) ;		// Quart voisin (1 eighth)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.0/8, 		51*Multiplicateur_NumeroMvt) ) ;		// 1er Eighth voisin (2 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(4.0/8,		61*Multiplicateur_NumeroMvt) ) ;		// 2è Eighth voisin (4 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(6.0/8,		65*Multiplicateur_NumeroMvt) ) ;		// 3è Eighth voisin (6 eighths) 
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1, 			70*Multiplicateur_NumeroMvt) ) ;		// 4è Eighth voisin (8 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1.5,	 		75*Multiplicateur_NumeroMvt) ) ;		// 6è Eighth voisin (12 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2,	 		80*Multiplicateur_NumeroMvt) ) ;		// 8è Eighth voisin (16 eighths)
				Vector_Bonus.push_back( Struct_Ligne_Bonus(2.5,	 		85*Multiplicateur_NumeroMvt) ) ;		// 10è Eighth voisin (20 eighths)
				Bonus_PlanA = Interpolation_Logarithmique (Vector_Bonus, RatioAmplitude_PlanA * 2) ;			// on multiplie par deux car PHJP-PBJP correspond à la distance entre deux Full
			}
		}
		else if ( SousNatureNiveau == 6 )			// PH MP ou PB MP
		{  
			if ( NumeroMouvement == 1 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		85) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		90) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		95) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	100) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	105) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	110) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	115) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	120) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	125) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	130) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	80) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	85) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	90) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	80) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		45) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	75) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	80) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
		else if ( SousNatureNiveau == 8 )			// Gap M
		{  
			if ( NumeroMouvement == 2 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		70+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		75+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		80+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		85+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		90+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		95+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	100+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	105+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	110+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	115+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	120+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	125+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	130+20) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 3 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		30+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		35+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		40+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		45+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		50+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		55+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	60+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	65+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	70+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	75+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	80+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	85+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	90+20) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 4 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		45+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	75+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	80+20) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
			else if ( NumeroMouvement == 5 )
			{
				Vector_Bonus.push_back( Struct_Ligne_Bonus(17, 		20+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(24, 		25+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(33,		30+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(46, 		35+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(65, 		40+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(92, 		45+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(130, 	50+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(183, 	55+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(260, 	60+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(370, 	65+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(520, 	70+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(730, 	75+20) ) ;
				Vector_Bonus.push_back( Struct_Ligne_Bonus(1050, 	80+20) ) ;
				
				Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
			}		
		}
		else if ( SousNatureNiveau == 10 )			// Plus Haut Absolu
		{   }
	}
	
	// Niveaux PH J et PB J
	else if ( NatureNiveau == 6 )			
	{	
		// quelque soit le mouvement
		Vector_Bonus.push_back( Struct_Ligne_Bonus(6, 		16) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(12, 		20) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(25, 		30) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(50, 		44) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(100, 	49) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(200,	53) ) ;
		Vector_Bonus.push_back( Struct_Ligne_Bonus(400,	60) ) ;
		
		Bonus_PlanB = Interpolation_Logarithmique (Vector_Bonus, Amplitude_pdm) ;
	}
	
	// { SCString message ;  message.Format("RatioAmplitude_PlanA = %.2f", RatioAmplitude_PlanA) ; ptrsc->AddMessageToLog(message,0) ; }
	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCString Conversion_Amplitude_pdm_En_SCString(float Amplitude_pdm)
{
	SCString Texte ;
	Amplitude_pdm = Amplitude_pdm / 10000 ;
	
	if 			( Amplitude_pdm < 30.0/10000 )  	{ Texte.Format("%.1f pdm", Amplitude_pdm*10000) ;   }  							// on arrondit à 1 chiffre derrière la virgule
	else if 	( Amplitude_pdm < 150.0/10000 )	{ Texte.Format("%.0f pdm", round(Amplitude_pdm*10000)) ;   }    			// on arrondit à l'entier
	else if 	( Amplitude_pdm < 300.0/10000 )	{ Texte.Format("%.0f pdm", round(Amplitude_pdm*2000)*5 );   }    			// on arrondit à 5
	else if 	( Amplitude_pdm < 2000.0/10000 )	{ Texte.Format("%.0f pdm", round(Amplitude_pdm*1000)*10 );   }    		// on arrondit à 10
	else 															{ Texte.Format("%.0f pdm", round(Amplitude_pdm*200)*50 );   }    			// on arrondit à 50
	
	return Texte ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCString Creation_Texte_Amplitude(float Amplitude_pdm, float RatioAmplitude_PlanA)
{
	SCString Texte, TexteAjoute ;
	
	// affichage en pdm
	Texte = Conversion_Amplitude_pdm_En_SCString(Amplitude_pdm) ;
	
	// // affichage en pts
	// if 			( Amplitude_pts < 30 )  	{ TexteAjoute.Format(" (%.1f pts)", Amplitude_pts) ;   }    					// on arrondit à 1 chiffre derrière la virgule
	// else if 	( Amplitude_pts < 150 )	{ TexteAjoute.Format(" (%.0f pts)", round(Amplitude_pts)) ;   }    		// on arrondit à l'entier
	// else if 	( Amplitude_pts < 300 )	{ TexteAjoute.Format(" (%.0f pts)", round(Amplitude_pts/5)*5 );   }    		// on arrondit à 5
	// else if 	( Amplitude_pts < 2000 )	{ TexteAjoute.Format(" (%.0f pts)", round(Amplitude_pts/10)*10 );   }   	// on arrondit à 10
	// else 											{ TexteAjoute.Format(" (%.0f pts)", round(Amplitude_pts/50)*50 );   }    	// on arrondit à 50
	// Texte.Append(TexteAjoute) ;
	
	// RatioAmplitude_PlanA
	if ( RatioAmplitude_PlanA > 0 ) { TexteAjoute.Format(" | %.2f Full", 2*RatioAmplitude_PlanA) ;  Texte.Append(TexteAjoute) ;  }
	
	return Texte ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCString Creation_Texte_Bonus(float Bonus_PlanA, float Bonus_PlanB, s_sc* ptrsc)
{
	SCString Texte ;
	SCInputRef 	Bonus_Plans_A_et_B_Input = ptrsc->Input[21];
	
	// Bonus PlanB
	if ( Bonus_PlanB > 0 )  { Texte.Format("Plan B ( %.0f )", round(Bonus_PlanB) ) ;   }  else Texte.Format("Plan B ( n.d. )") ; 

	// Bonus PlanA
	if ( Bonus_PlanA > 0 )  { Texte.AppendFormat(" + A ( %.0f )", round(Bonus_PlanA) ) ;   } 
	
	// Moyenne
	if ( Bonus_PlanB > 0  and  Bonus_PlanA > 0 )  
	{ 
		float Bonus_Plans_A_et_B = round((Bonus_PlanB+Bonus_PlanA)/2) ;
		Texte.AppendFormat(" = %.0f", Bonus_Plans_A_et_B ) ;   
		Bonus_Plans_A_et_B_Input.SetFloat(Bonus_Plans_A_et_B) ;
	} 
	else { Bonus_Plans_A_et_B_Input.SetFloat(Bonus_PlanB) ;  }

	return Texte ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Button_18_Amplitude_And_Button_19_Bonus (SCString Texte, float Amplitude_pdm, float RatioAmplitude_PlanA, float Bonus_PlanA, float Bonus_PlanB, s_sc* ptrsc)		// Amplitude et Bonus
{
	SCInputRef 	NatureNiveau_Input = ptrsc->Input[3];  
	int NatureNiveau = NatureNiveau_Input.GetInt() ;
	
	if ( ( ptrsc->ChartNumber == 15 and NatureNiveau != 0 and NatureNiveau != 5)  or ( ptrsc->ChartNumber == 19 and ( NatureNiveau == 0 or NatureNiveau == 5 ) ) )    	// Niveaux psys et M avec le chart 19 
	{ 
		// Button 18
		SCString Texte_Button18 ;
		if ( Texte == "" ) Texte_Button18 = Creation_Texte_Amplitude(Amplitude_pdm, RatioAmplitude_PlanA) ;  else Texte_Button18 = Texte ;
		
		// TextePrecedent_Button18
		int& StudyID_QuantiemeMvt_Chart15 = ptrsc->GetPersistentIntFast(14); 
		SCString TextePrecedent_Button18 = ptrsc->GetPersistentSCStringFromChartStudy(15, StudyID_QuantiemeMvt_Chart15, 1) ;			// GetPersistentSCStringFromChartStudy(int ChartNumber, int StudyID, int Key);
		
		// Modif de TextePrecedent_Button18
		if ( Texte_Button18 != TextePrecedent_Button18 )
		{
			ptrsc->SetCustomStudyControlBarButtonText(18, Texte_Button18) ; 		
			ptrsc->SetPersistentSCStringForChartStudy(15, StudyID_QuantiemeMvt_Chart15, 1, Texte_Button18) ;			// void SetPersistentSCStringForChartStudy(int ChartNumber, int StudyID, int Key, SCString Value);
		}


		// Button 19
		SCString Texte_Button19 ;
		if ( Texte == "" ) Texte_Button19 = Creation_Texte_Bonus(Bonus_PlanA, Bonus_PlanB, ptrsc) ;  else Texte_Button19 = Texte ;
		
		// TextePrecedent_Button19
		SCString TextePrecedent_Button19 = ptrsc->GetPersistentSCStringFromChartStudy(15, StudyID_QuantiemeMvt_Chart15, 2) ;			// GetPersistentSCStringFromChartStudy(int ChartNumber, int StudyID, int Key);
		
		// Modif de TextePrecedent_Button19
		if ( Texte_Button19 != TextePrecedent_Button19 )
		{
			ptrsc->SetCustomStudyControlBarButtonText(19, Texte_Button19) ; 		
			ptrsc->SetPersistentSCStringForChartStudy(15, StudyID_QuantiemeMvt_Chart15, 2, Texte_Button19) ;
		}
	}
	
	// Pour les niveaux psys du chart #15, on modifie également Bonus_Plans_A_et_B_Input 
	if ( /* ptrsc->ChartNumber == 15  and   */NatureNiveau == 0 )
	{
		SCInputRef 	Bonus_Plans_A_et_B_Input = ptrsc->Input[21] ;
		Bonus_Plans_A_et_B_Input.SetFloat(Bonus_PlanB) ;
	}
	
	// Amplitude_pdm est écrite dans Amplitude_pdm_Input
	SCInputRef 	Amplitude_pdm_Input = ptrsc->Input[22];	
	Amplitude_pdm_Input.SetFloat(Amplitude_pdm) ;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_QuantiemeMvt(SCStudyInterfaceRef sc)
{

	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef Amplitude_pdm_Seuil_Input = sc.Input[0];												// normé à 10000 pdm
	SCInputRef 	NomNiveau_Input = sc.Input[1];      				// modifié par AMR
	SCInputRef 	ValeurNiveau_Input = sc.Input[2];      				// modifié par AMR
	SCInputRef 	NatureNiveau_Input = sc.Input[3];      				// modifié par AMR			// appelé par SetButton22_NumeroMouvement et Set_Button_18_Amplitude_And_Button_19_Bonus
	SCInputRef 	SousNatureNiveau_Input = sc.Input[4];  		// modifié par AMR
	SCInputRef 	TriggerRecalculate = sc.Input[5];    		  		// modifié par AMR
	SCInputRef 	IndexHoraireRAZ_Input = sc.Input[8];    			// modifié par AMR
	SCInputRef 	AffichageONOFF_Input = sc.Input[9];    			// modifié par AMR
	SCInputRef 	Distance_pdm_Effleurage_Input = sc.Input[10]; 												// normé à 10000 pdm
	SCInputRef 	DebugDansMessageLogONOFF_Input = sc.Input[12]; 
	SCInputRef 	AffichageLignesSeuilEtEffleurement_Input = sc.Input[13]; 
	SCInputRef 	RangePrecedent_Input = sc.Input[20];	  		// modifié par AMR
	SCInputRef 	Bonus_Plans_A_et_B_Input = sc.Input[21];	// appelé par AMR		// modifié par Creation_Texte_Bonus		// modifié par Set_Button_18_Amplitude_And_Button_19_Bonus
	SCInputRef 	Amplitude_pdm_Input = sc.Input[22];				// appelé par AMR		// modifié par Set_Button_18_Amplitude_And_Button_19_Bonus			// cet input permet à AMR de récupérer l'Amplitude_pdm du mvt tradé
	
	
	struct StructureMouvement
	{
		// variables membres :
		int NumeroMouvement ;
		int TypeMouvement ;		// 1 = haussier		// -1 = baissier
		float Amplitude_pdm ;		// normé à 10000
		int IndexCrossing ;			// Index où le niveau est traversé
		int IndexExtremum ;			// Index du maximum pour un mvt baissier, resp. du minimum pour un mvt haussier
		
		// Constructeur par défaut
		StructureMouvement()
		{
			NumeroMouvement = -999 ;
			TypeMouvement = -999 ;
			Amplitude_pdm = -999 ;
			IndexCrossing = -999 ;
			IndexExtremum = -999 ;
		}
	} ;




	// Références persistantes
	int& TypeMouvementInitial = sc.GetPersistentIntFast(0); 			// appelé par IncrementationComptageMouvements		// 0 = n.d.		// 1 = haussier		// -1 = baissier
	int& MouvementNumero1 = sc.GetPersistentIntFast(1);  			// booleen de premier mouvement
	int& PreviousLowest_Index = sc.GetPersistentIntFast(2);  
	int& PreviousHighest_Index = sc.GetPersistentIntFast(3);  
	int& IndexFinPeriode = sc.GetPersistentIntFast(4);  
	int& bool_InsideOpenGap = sc.GetPersistentIntFast(5);  					// booléen qui indique la présente d'un open gap
	int& IndexPreviousCrossing = sc.GetPersistentIntFast(6);
	int& Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine = sc.GetPersistentIntFast(7);
	int& StudyID_PivotsJ = sc.GetPersistentIntFast(11);  
	int& StudyID_PivotsH= sc.GetPersistentIntFast(12);  
	int& StudyID_PivotsM = sc.GetPersistentIntFast(13);  
	int& StudyID_QuantiemeMvt_Chart15 = sc.GetPersistentIntFast(14);  		// appelé par SetButton22, SetButton18 et SetButton19
	int& StudyID_PrixTypique_Chart6 = sc.GetPersistentIntFast(15); 
	
	float& PreviousLowest = sc.GetPersistentFloatFast(0);  
	float& PreviousHighest = sc.GetPersistentFloatFast(1);  
	float& MouvementRelatif_Seuil = sc.GetPersistentFloatFast(2);  			// normé à 1
	float& Epsilon = sc.GetPersistentFloatFast(3);  		// normé à 10000
	
	SCString& TextePrecedent_Button22 = sc.GetPersistentSCString(0);  	// appelé par SetButton22_NumeroMouvement
	SCString& TextePrecedent_Button18 = sc.GetPersistentSCString(1);  	// appelé par SetButton18
	SCString& TextePrecedent_Button19 = sc.GetPersistentSCString(2);  	// appelé par SetButton19

	std::vector<StructureMouvement> * v_Mouvements = (std::vector<StructureMouvement> *) sc.GetPersistentPointer(2);		// v_Mouvements est un pointeur sur un conteneur (vecteur) de StructureMouvement		// pour une raison incompréhensible la key de GetPersistentPointer doit être égale à 2


	// Variables
	int const IndexHoraireRAZ = IndexHoraireRAZ_Input.GetInt() ;
	int const NatureNiveau= NatureNiveau_Input.GetInt();
	int const LastIndex = sc.ArraySize - 1 ;
	
	float const Niveau = ValeurNiveau_Input.GetFloat() ;
	float const Interligne = ( sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) - sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ) / 300 ;   // c'est la valeur en points entre deux lignes de textes affichées sur le graphique  

	int IndexDebutPeriode = 0 ;
	int NoRecalculate = 1 ;

	s_sc* const ptrsc = &sc;  
	
	COLORREF orange=RGB(255,128,0) ;		  
	COLORREF marron=RGB(153,76,0) ;		  
	
	
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Quantieme Mvt";		// appelé par elle-même (Chart 19 appelle la study du chart 15)
		sc.AutoLoop = 0; 
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;		
		sc.HideStudy = 1 ;
		
		// Liste des inputs
		Amplitude_pdm_Seuil_Input.Name = "Mouvement Seuil (pdm)" ;
		Amplitude_pdm_Seuil_Input.SetFloat(15) ;
		NomNiveau_Input.Name = "Nom du niveau";
		NomNiveau_Input.SetString("Nom") ;
		ValeurNiveau_Input.Name = "Prix du niveau (pts)";
		ValeurNiveau_Input.SetFloat(10700);   
		NatureNiveau_Input.Name = "NatureNiveau";
		NatureNiveau_Input.SetInt(0);
		SousNatureNiveau_Input.Name = "SousNatureNiveau";
		SousNatureNiveau_Input.SetInt(0);
		TriggerRecalculate.Name = "TriggerRecalculate";
		TriggerRecalculate.SetYesNo(0);
		IndexHoraireRAZ_Input.Name = "IndexHoraireRAZ_Input";
		IndexHoraireRAZ_Input.SetInt(0);
		AffichageONOFF_Input.Name = "AffichageONOFF (idem AMR)";  // gère l'affichage (même état que AffichageONOFF de AMR)
		AffichageONOFF_Input.SetYesNo(1);
		Distance_pdm_Effleurage_Input.Name = "Distance Effleurage (pdm)";
		Distance_pdm_Effleurage_Input.SetFloat(2);   
		DebugDansMessageLogONOFF_Input.Name = "Debug Dans MessageLog ON/OFF";
		DebugDansMessageLogONOFF_Input.SetYesNo(0);
		AffichageLignesSeuilEtEffleurement_Input.Name = "Affichage lignes Seuil et Effleurement";
		AffichageLignesSeuilEtEffleurement_Input.SetYesNo(0);
		RangePrecedent_Input.Name = "Range Precedent (pts)";
		RangePrecedent_Input.SetFloat(0);
		Bonus_Plans_A_et_B_Input.Name = "Bonus_Plans_A_et_B";
		Bonus_Plans_A_et_B_Input.SetFloat(0);
		Amplitude_pdm_Input.Name = "Amplitude_pdm" ;
		Amplitude_pdm_Input.SetFloat(0);
		
		
		// DisplayOrder des inputs
		NomNiveau_Input.DisplayOrder = 1 ;
		ValeurNiveau_Input.DisplayOrder = 2 ;
		NatureNiveau_Input.DisplayOrder = 3 ;
		SousNatureNiveau_Input.DisplayOrder = 4 ;
		Amplitude_pdm_Input.DisplayOrder = 5 ;
		Bonus_Plans_A_et_B_Input.DisplayOrder = 6 ;
		RangePrecedent_Input.DisplayOrder = 7 ;
		Amplitude_pdm_Seuil_Input.DisplayOrder = 8 ;
		Distance_pdm_Effleurage_Input.DisplayOrder = 9 ;
		
		return;
	}
	
	
	if ( sc.UpdateStartIndex == 0  or  TriggerRecalculate.GetYesNo() == 1 ) 
	{
		// On RAZ le TriggerRecalculate
		TriggerRecalculate.SetYesNo(0);
		NoRecalculate = 0 ; 			// multuplicateur de sc.UpdateStartIndex

		// Initialisations des variables
		sc.GraphRegion = 0; 
		MouvementRelatif_Seuil = Amplitude_pdm_Seuil_Input.GetFloat() / 10000 ;	
		Epsilon = Distance_pdm_Effleurage_Input.GetFloat() / 10000 * Niveau ;		// en points
		TypeMouvementInitial = 0 ;
		bool_InsideOpenGap = 0 ;
		IndexFinPeriode = 9999999 ;
		IndexPreviousCrossing = -1 ;
		MouvementNumero1 = true ;
		Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine = false ;		// true pour les backtests, false pour le trading en temps réel
		TextePrecedent_Button22 = "" ;  TextePrecedent_Button18 = "" ;  TextePrecedent_Button19 = "" ;
		if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message = "----------------------------------------------------------------------------------------------------" ;  sc.AddMessageToLog(message,0) ;  }
		
		// Zeroing tools
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		

		
		// Tracé de Niveau et des niveaux seuils
		if ( AffichageLignesSeuilEtEffleurement_Input.GetYesNo() )
		{
			creationLignePointilleMarron (Niveau*(1+MouvementRelatif_Seuil), 1, marron, ptrsc) ;	
			creationLignePointilleMarron (Niveau*(1-MouvementRelatif_Seuil), 1, marron, ptrsc) ;	
			creationLignePointilleMarron (Niveau+Epsilon, 1, marron, ptrsc) ;	
			creationLignePointilleMarron (Niveau-Epsilon, 1, marron, ptrsc) ;	
		}
		
		// Allocation de mémoire pour v_Mouvements
		if (v_Mouvements == NULL)
		{
			v_Mouvements = new std::vector<StructureMouvement> ;

			if (v_Mouvements != NULL)  sc.SetPersistentPointer(2, v_Mouvements);
			else { sc.AddMessageToLog("Memory allocation error : v_Mouvements", 1);  return;  }
		}
		
		// le contenu de v_Mouvements doit être effacé, sinon à chaque recalcul les push_back s'ajoutent à ceux des précédents recalculs
		v_Mouvements->clear() ;

		// Récupération de la StudyID_PivotsJ pour l'OpenGap
		StudyID_PivotsJ = sc.GetStudyIDByName(sc.ChartNumber, "[6] Pivots Ind J (Full/Mid/Quart)", 0) ;
		if  (StudyID_PivotsJ == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''[6] Pivots Ind J (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsH pour l'OpenGap
		StudyID_PivotsH = sc.GetStudyIDByName(sc.ChartNumber, "[4] Pivots Ind H (Full/Mid/Quart)", 0) ;
		if  (StudyID_PivotsH == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''[4] Pivots Ind H (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsM pour l'OpenGap
		StudyID_PivotsM = sc.GetStudyIDByName(sc.ChartNumber, "[1] Pivots Ind M (Full/Mid/Quart)", 0) ;
		if  (StudyID_PivotsM == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''[1] Pivots Ind M (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }
	
		// Récupération de la StudyID_QuantiemeMvt_Chart15
		{
			StudyID_QuantiemeMvt_Chart15 = sc.GetStudyIDByName(15, "Quantieme Mvt", 0) ;
			if  (StudyID_QuantiemeMvt_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! Le graphique #15 ne contient pas la study ''Quantieme Mvt''") ; sc.AddMessageToLog(message,1) ; }
		}

		// Récupération de la StudyID_PrixTypique_Chart6
		{
			StudyID_PrixTypique_Chart6 = sc.GetStudyIDByName(6, "Prix typique", 0) ;
			if  (StudyID_PrixTypique_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! Le graphique #6 ne contient pas la study ''Prix Typique''") ; sc.AddMessageToLog(message,1) ; }
		}

	
		// Recherche de IndexDebutPeriode : premier index pour la période du niveau tradé. On décrémente index en partant de IndexHoraireRAZ
		{
			int index = IndexHoraireRAZ ;
			
			if ( NatureNiveau == 0 )  // Psy : on cherche la dernière fois que le Niveau a été traversé
			{
				while ( index > 0 )
				{
					if ( sc.BaseDateTimeIn[index].GetDate()  !=  sc.BaseDateTimeIn[index-1].GetDate() )  break ;
					index -- ;
				}

				// Initialisations
				IndexDebutPeriode = index ;
				PreviousLowest = sc.Low[IndexDebutPeriode] ;
				PreviousHighest = sc.High[IndexDebutPeriode] ;
				PreviousLowest_Index = IndexDebutPeriode ;
				PreviousHighest_Index = IndexDebutPeriode ;
				
				// on balaye les séances avant la séance actuelle  (index vaut d'abord IndexDebutPeriode-1)
				index = IndexDebutPeriode - 1 ;
				while ( index > 0 )
				{
					// Variables recalculées sur chaque bougies
					if ( sc.Low[index] < PreviousLowest )
					{
						PreviousLowest = sc.Low[index] ;
						PreviousLowest_Index = index ;
					}
					if ( sc.High[index] > PreviousHighest )
					{
						PreviousHighest = sc.High[index] ;
						PreviousHighest_Index = index ;
					}
					// Conditions d'arrêt
					if ( sc.Low[index-1] > Niveau+Epsilon  and  sc.Low[index] <= Niveau+Epsilon ) break ;  		// on a dépassé le niveau par un mvt baissier
					if ( sc.High[index-1] < Niveau-Epsilon  and  sc.High[index] >= Niveau-Epsilon )  break ;		// on a dépassé le niveau par un mvt haussier
					index -- ;
				}
				
				// Récupération du SCFloatArray externe OpenGap J
				SCFloatArray OpenGap ;
				sc.GetStudyArrayUsingID(StudyID_PivotsJ, 0, OpenGap);			
				float PrixOpenGap = OpenGap[IndexDebutPeriode] ;
				if ( PrixOpenGap !=0 )
				{
					// le niveau est-il à l'intérieur de l'OpenGap ?  (et donc le mvt 1 est inclu dans l'open gap)
					if ( sc.High[IndexDebutPeriode] > Niveau-Epsilon  and  PrixOpenGap < Niveau+Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap haussier
					if ( sc.Low[IndexDebutPeriode] < Niveau+Epsilon  and  PrixOpenGap > Niveau-Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap baissier
				}
			}
			
			else if ( NatureNiveau == 1 )  // PP 1h
			{
				if ( sc.ChartNumber == 15 )			// le calcul de QuantiemeMvt pour un PP 1h ou 4h n'est réalisé que dans la fenêtre #15 (UT 20s). Ce n'est pas pertinent pour le chart #19.
				{
					if ( sc.ChartWindowIsActive )  index = sc.IndexOfLastVisibleBar ;			
					else 	// on récupère SCDT_DernierChandelierVisible dans le chart #6 pour en déduire index
					{
						SCDateTime SCDT_DernierChandelierVisible_Chart6 ;
						SCDT_DernierChandelierVisible_Chart6 = sc.GetPersistentSCDateTimeFromChartStudy(6, StudyID_PrixTypique_Chart6, 0) ;		// SCDateTime& GetPersistentSCDateTimeFromChartStudy(int ChartNumber, int StudyID, int Key);
						index = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, SCDT_DernierChandelierVisible_Chart6);
					}
					
					int heure = sc.BaseDateTimeIn[index].GetHour() ;
					if ( heure == 9  or  heure == 16 )
					{ sc.AddMessageToLog("!! WARNING !! L'heure de IndexOfLastVisibleBar n'est pas compatible avec un PP 1h", 1) ;  }
					else 
					{
						while ( index > 0 )
						{
							if ( sc.BaseDateTimeIn[index].GetHour()  !=  sc.BaseDateTimeIn[index-1].GetHour() )  break ;
							index -- ;
						}
					}
					// Initialisations
					IndexDebutPeriode = index ;
					PreviousLowest = sc.Low[index] ;
					PreviousHighest = sc.High[index] ;
					PreviousLowest_Index = IndexDebutPeriode ;
					PreviousHighest_Index = IndexDebutPeriode ;
				}
			}
			
			else if ( NatureNiveau == 2 )  // PP 4h			
			{
				if ( sc.ChartNumber == 15 )			// le calcul de QuantiemeMvt pour un PP 1h ou 4h n'est réalisé que dans la fenêtre #15 (UT 20s). Ce n'est pas pertinent pour le chart #19.
				{
					if ( sc.ChartWindowIsActive )  index = sc.IndexOfLastVisibleBar ;			
					else 	// on récupère SCDT_DernierChandelierVisible dans le chart #6 pour en déduire index
					{
						SCDateTime SCDT_DernierChandelierVisible_Chart6 ;
						SCDT_DernierChandelierVisible_Chart6 = sc.GetPersistentSCDateTimeFromChartStudy(6, StudyID_PrixTypique_Chart6, 0) ;		// SCDateTime& GetPersistentSCDateTimeFromChartStudy(int ChartNumber, int StudyID, int Key);
						index = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, SCDT_DernierChandelierVisible_Chart6);
					}

					int heure = sc.BaseDateTimeIn[index].GetHour() ;
					if ( heure < 13  or  heure > 16 )
					{ sc.AddMessageToLog("!! WARNING !! L'heure de IndexOfLastVisibleBar n'est pas compatible avec un PP 4h", 1) ;  }
					else 
					{
						while ( index > 0 )
						{
							if ( sc.BaseDateTimeIn[index-1].GetHour() < 13 )  break ;
							index -- ;
						}
					}
					// Initialisations
					IndexDebutPeriode = index ;
					PreviousLowest = sc.Low[index] ;
					PreviousHighest = sc.High[index] ;
					PreviousLowest_Index = IndexDebutPeriode ;
					PreviousHighest_Index = IndexDebutPeriode ;
				}
			}
			
			else if ( NatureNiveau == 3  or  NatureNiveau == 6 )  // Jour : on cherche le premier index du jour
			{
				while ( index > 0 )
				{
					if ( sc.BaseDateTimeIn[index].GetDate()  !=  sc.BaseDateTimeIn[index-1].GetDate() )  break ;
					index -- ;
				}
				// Initialisations
				IndexDebutPeriode = index ;
				PreviousLowest = sc.Low[index] ;
				PreviousHighest = sc.High[index] ;
				PreviousLowest_Index = IndexDebutPeriode ;
				PreviousHighest_Index = IndexDebutPeriode ;

				// Récupération du SCFloatArray externe OpenGap
				SCFloatArray OpenGap ;
				sc.GetStudyArrayUsingID(StudyID_PivotsJ, 0, OpenGap);			
				float PrixOpenGap = OpenGap[index] ;
				if ( PrixOpenGap !=0 )
				{
					PreviousLowest = std::fmin(PreviousLowest, OpenGap[index]) ;
					PreviousHighest = std::fmax(PreviousHighest, OpenGap[index]) ;
					
					// le niveau est-il à l'intérieur de l'OpenGap ?  (et donc le mvt 1 est inclu dans l'open gap)
					if ( PreviousHighest > Niveau-Epsilon  and  PrixOpenGap < Niveau+Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap haussier
					if ( PreviousLowest < Niveau+Epsilon  and  PrixOpenGap > Niveau-Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap baissier
				}
			}
			
			else if ( NatureNiveau == 4 )  // Hebdo : on cherche le premier index de la semaine
			{
				while ( index > 0 )
				{
					if ( sc.BaseDateTimeIn[index].GetDayOfWeek()  <  sc.BaseDateTimeIn[index-1].GetDayOfWeek() )  break ;
					index -- ;
				}
				// Initialisations
				IndexDebutPeriode = index ;
				PreviousLowest = sc.Low[index] ;
				PreviousHighest = sc.High[index] ;
				PreviousLowest_Index = IndexDebutPeriode ;
				PreviousHighest_Index = IndexDebutPeriode ;
				// Récupération du SCFloatArray externe OpenGap
				SCFloatArray OpenGap ;
				sc.GetStudyArrayUsingID(StudyID_PivotsH, 0, OpenGap);			
				float PrixOpenGap = OpenGap[index] ;
				if ( PrixOpenGap !=0 )
				{
					PreviousLowest = std::fmin(PreviousLowest, OpenGap[index]) ;
					PreviousHighest = std::fmax(PreviousHighest, OpenGap[index]) ;
					
					// le niveau est-il à l'intérieur de l'OpenGap ?  (et donc le mvt 1 est inclu dans l'open gap)
					if ( PreviousHighest > Niveau-Epsilon  and  PrixOpenGap < Niveau+Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap haussier
					if ( PreviousLowest < Niveau+Epsilon  and  PrixOpenGap > Niveau-Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap baissier
				}
			}
			
			else if ( NatureNiveau == 5 )  // Mensuel : on cherche le premier index du mois
			{
				while ( index > 0 )
				{
					if ( sc.BaseDateTimeIn[index].GetMonth() !=  sc.BaseDateTimeIn[index-1].GetMonth() )  break ;
					index -- ;
				}
				// Initialisations
				IndexDebutPeriode = index ;
				PreviousLowest = sc.Low[index] ;
				PreviousHighest = sc.High[index] ;
				PreviousLowest_Index = IndexDebutPeriode ;
				PreviousHighest_Index = IndexDebutPeriode ;
				// Récupération du SCFloatArray externe OpenGap
				SCFloatArray OpenGap ;
				sc.GetStudyArrayUsingID(StudyID_PivotsM, 0, OpenGap);			
				float PrixOpenGap = OpenGap[index] ;
				if ( PrixOpenGap !=0 )
				{
					PreviousLowest = std::fmin(PreviousLowest, OpenGap[index]) ;
					PreviousHighest = std::fmax(PreviousHighest, OpenGap[index]) ;
					
					// le niveau est-il à l'intérieur de l'OpenGap ?  (et donc le mvt 1 est inclu dans l'open gap)
					if ( PreviousHighest > Niveau-Epsilon  and  PrixOpenGap < Niveau+Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap haussier
					if ( PreviousLowest < Niveau+Epsilon  and  PrixOpenGap > Niveau-Epsilon ) bool_InsideOpenGap = 1 ;			// le niveau est à l'intérieur d'un open gap baissier
				}
			}
			
			// Marque sur IndexDebutPeriode
			{ SCString Texte= "Debut Periode " ;  DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, Niveau, 9, marron, DT_VCENTER | DT_RIGHT, ptrsc) ;  }
			DrawMarker_UTAMAOA (MARKER_TRIANGLERIGHT, IndexDebutPeriode-1, Niveau, 9, marron, ptrsc) ;    
		}
	}
	
	
	for (int index = max(IndexDebutPeriode+1, sc.UpdateStartIndex*NoRecalculate) ; index <  min (sc.ArraySize, IndexFinPeriode) ; index++)   // début AutoLoop
	{
		
		// Recherche de IndexFinPeriode : dernier index pour la période du niveau tradé			// bloc à placer en début de autoloop sinon on peut avoir un mouvement détecté à l'open de la période suivante
		{
			if ( NatureNiveau == 0  or  NatureNiveau == 3  or  NatureNiveau == 6 )  // Psy ou Jour ou PHJ/PBJ : on cherche le dernier index de la séance
			{
				if ( sc.BaseDateTimeIn[index].GetDate()  !=  sc.BaseDateTimeIn[index-1].GetDate()  and  index > IndexHoraireRAZ ) 
				{
					IndexFinPeriode = index-1 ;		// une nouvelle séance a débuté à index
				}
			}
			else if ( NatureNiveau == 4 )  // Hebdo : on cherche le dernier index de la semaine
			{
				if ( sc.BaseDateTimeIn[index].GetDayOfWeek()  <  sc.BaseDateTimeIn[index-1].GetDayOfWeek()  and  index > IndexHoraireRAZ ) 
				{
					IndexFinPeriode = index-1 ;		// une nouvelle séance a débuté à index
				}
			}
			else if ( NatureNiveau == 5 )  // Mensuel : on cherche le dernier index du mois
			{
				if ( sc.BaseDateTimeIn[index].GetMonth()  !=  sc.BaseDateTimeIn[index-1].GetMonth()  and  index > IndexHoraireRAZ ) 
				{
					IndexFinPeriode = index-1 ;		// une nouvelle séance a débuté à index
				}
			}
			else if ( NatureNiveau == 1 )  // PP 1h : on cherche le dernier index de l'heure
			{
				if ( sc.BaseDateTimeIn[index].GetHour()  !=  sc.BaseDateTimeIn[index-1].GetHour()  and  index > IndexHoraireRAZ ) 
				{
					IndexFinPeriode = index-1 ;		// une nouvelle heure a débuté à index
				}
			}
			else if ( NatureNiveau == 2 )  // PP 4h : on cherche 16h
			{
				if ( sc.BaseDateTimeIn[index].GetHour()  > 15  and  index > IndexHoraireRAZ ) 
				{
					IndexFinPeriode = index-1 ;		// il est 16h à index
				}
			}

			// Marque sur IndexCloture (16h08) et return
			if ( index == IndexFinPeriode+1 )
			{
				// IndexCloture
				int IndexCloture ;
				if ( NatureNiveau == 0  or  NatureNiveau == 3  or  NatureNiveau == 4  or  NatureNiveau == 5  or  NatureNiveau == 6 )		// le texte "Fin Periode" est tracé à 16h08
				{
					SCDateTime DateTimeCloture = SCDateTime(sc.BaseDateTimeIn[IndexFinPeriode].GetDate(), 16*3600 +8*60) ;			// SCDateTime(int DateValue, int TimeValue);
					IndexCloture = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, DateTimeCloture);
				}				
				else IndexCloture = IndexFinPeriode ;
				
				// Draw
				SCString Texte = " Fin Periode" ;  DrawText_UTAMAOA (Texte, IndexCloture, Niveau, 9, marron, DT_VCENTER | DT_LEFT, ptrsc) ;
				DrawMarker_UTAMAOA (MARKER_TRIANGLELEFT, IndexCloture, Niveau, 9, marron, ptrsc) ;    
				return ;
			}
		}
		
		// cas particulier 1 : la bougie d'IndexDebutPeriode inclut Niveau, alors on a déjà réalisé le mvt 1
		if ( index == IndexDebutPeriode+1  and  sc.High[IndexDebutPeriode] >= Niveau-Epsilon  and  sc.Low[IndexDebutPeriode] <= Niveau+Epsilon  )  
		{
			bool_InsideOpenGap = 0 ;
			float AmplitudeActuelle_pdm = 0 ;
			MouvementNumero1 = false ;			// on met le bool à zéro car on est sur le mvt initial
			TypeMouvementInitial = -2 ;

			// On calcule NumeroMouvement pour le mvt actuel
			int NumeroMouvement = 1 ;
			if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement = %d", NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;  }
			
			// Tracés sur le graphe
			DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexDebutPeriode, Niveau+1*Interligne, 9, orange, ptrsc) ;       						// void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
			DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexDebutPeriode, Niveau-1*Interligne, 9, orange, ptrsc) ;       				// void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
			SCString Texte ;  Texte.Format("Mvt %d\n", NumeroMouvement) ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;
			DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, Niveau, 9, orange, DT_VCENTER | DT_RIGHT, ptrsc) ;
	
			// v_Mouvements
			StructureMouvement ObjetMvt ;
			ObjetMvt.	NumeroMouvement = NumeroMouvement ;	
			ObjetMvt.	TypeMouvement = TypeMouvementInitial ;
			ObjetMvt.	Amplitude_pdm = AmplitudeActuelle_pdm ;
			ObjetMvt.	IndexCrossing = index ;
			ObjetMvt.	IndexExtremum = index ;
			v_Mouvements->push_back(ObjetMvt) ;
		
			// RAZ des variables
			PreviousLowest = Niveau ;
			PreviousHighest = Niveau ;
			PreviousLowest_Index = index ;
			PreviousHighest_Index = index ;
			IndexPreviousCrossing = index ;
		}

		// cas particulier 2 : opengap
		if ( index == IndexDebutPeriode+1  and  bool_InsideOpenGap )  
		{
			bool_InsideOpenGap = 0 ;
			MouvementNumero1 = false ;			// on met le bool à zéro car on est sur le mvt initial
			float AmplitudeActuelle_pdm = 0 ;
			
			// On calcule NumeroMouvement pour le mvt actuel
			int NumeroMouvement = 1 ;
			if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement = %d", NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;  }
			
			if ( sc.High[IndexDebutPeriode] >= Niveau-Epsilon ) 				// open gap haussier
			{
				AmplitudeActuelle_pdm = (sc.High[IndexDebutPeriode] - PreviousLowest)/Niveau*10000 ;
				TypeMouvementInitial = 1 ;
				/* // - sur Niveau :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexDebutPeriode, Niveau+1*Interligne, 9, orange, ptrsc) ;       						// void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
				SCString Texte ;  Texte.Format("Mvt %d\n", NumeroMouvement) ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;
				DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, Niveau, 9, orange, DT_VCENTER | DT_RIGHT, ptrsc) ;		// PreviousLowest correspond à OpenGap
				// - sur PreviousLowest :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexDebutPeriode, PreviousLowest-1*Interligne, 9, marron, ptrsc) ;       		
				Texte.Format("%.0f pdm\nDebut Mvt 1", AmplitudeActuelle_pdm) ;
				DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, PreviousLowest+1*Interligne, 9, marron, DT_TOP | DT_RIGHT, ptrsc) ; */
			}
			else if ( sc.Low[IndexDebutPeriode] <= Niveau+Epsilon )			// open gap baissier
			{ 
				AmplitudeActuelle_pdm = (PreviousHighest - sc.Low[IndexDebutPeriode])/Niveau*10000 ;  
				TypeMouvementInitial = -1 ;
				/* // - sur Niveau :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexDebutPeriode, Niveau-1*Interligne, 9, orange, ptrsc) ;       				// void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
				SCString Texte ;  Texte.Format("Mvt %d\n", NumeroMouvement) ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;
				DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, Niveau, 9, orange, DT_VCENTER | DT_RIGHT, ptrsc) ;	
				// - sur PreviousHighest :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexDebutPeriode, PreviousHighest+1*Interligne, 9, marron, ptrsc) ;       		
				Texte.Format("Debut Mvt 1\n%.0f pdm", AmplitudeActuelle_pdm) ;
				DrawText_UTAMAOA (Texte, IndexDebutPeriode-1, PreviousHighest+1*Interligne, 9, marron, DT_BOTTOM | DT_RIGHT, ptrsc) ; */
			}

	
			// v_Mouvements
			StructureMouvement ObjetMvt ;
			ObjetMvt.	NumeroMouvement = NumeroMouvement ;	
			ObjetMvt.	TypeMouvement = TypeMouvementInitial ;
			ObjetMvt.	Amplitude_pdm = AmplitudeActuelle_pdm ;
			ObjetMvt.	IndexCrossing = index ;
			ObjetMvt.	IndexExtremum = index ;
			v_Mouvements->push_back(ObjetMvt) ;
		
			// RAZ des variables
			PreviousLowest = Niveau ;
			PreviousHighest = Niveau ;
			PreviousLowest_Index = index ;
			PreviousHighest_Index = index ;			
			IndexPreviousCrossing = index ;
		}
		
		// cas général 1 : si on vient de dépasser Niveau : MVT BAISSIER
		else if ( sc.Low[index-1] > Niveau+Epsilon  and  sc.Low[index] <= Niveau+Epsilon ) 
		{
			float AmplitudeActuelle_pdm = (PreviousHighest-Niveau)/Niveau*10000 ;
			if ( AmplitudeActuelle_pdm > MouvementRelatif_Seuil*10000  or  MouvementNumero1 )  		// le mvt est supérieur à x pdm  OU  c'est le premier passage
			{
				// si le mouvement précédent était indéterminé, on rectifie cette indétermination ici
				if ( TypeMouvementInitial == -2 )  
				{
					TypeMouvementInitial = 1 ;	// le mvt initial était haussier
					v_Mouvements->begin()->TypeMouvement = TypeMouvementInitial ;
				}

				// Pour trouver NumeroMouvementPrecedent, on itère sur les mvts précédents (en partant du dernier) tant que le mouvement actuel est plus important que les précédents  		
				int NumeroMouvementPrecedent = 999 ;
				std::vector<StructureMouvement>::iterator v_Mouvements_iterator = v_Mouvements->end() ;
				bool nobreak = true ;		// reste vrai si on breake pas durant la boucle while
				while ( v_Mouvements_iterator != v_Mouvements->begin() )			// pour le mouvement 1, v_Mouvements est de taille 0, donc la boucle while est interrompue au test initial
				{
					-- v_Mouvements_iterator ;
					if ( ( v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm )  or  ( v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm ) )	// le mvt actuel n'est pas plus important que ce mouvement précédent 
					{
						NumeroMouvementPrecedent = min(NumeroMouvementPrecedent, v_Mouvements_iterator->NumeroMouvement) ;		
						
						if ( DebugDansMessageLogONOFF_Input.GetYesNo() )
						{
							SCString message ;  
							message.Format("--------------------") ;  sc.AddMessageToLog(message,0) ;
							message.Format("AmplitudeActuelle_pdm = %5.0f pdm | NumeroMouvementPrecedent = %d", AmplitudeActuelle_pdm, NumeroMouvementPrecedent) ;  sc.AddMessageToLog(message,0) ;
							message.Format("v_Mouvements_iterator->Amplitude_pdm (important) = %5.0f | v_Mouvements_iterator->NumeroMouvement = %d", v_Mouvements_iterator->Amplitude_pdm, v_Mouvements_iterator->NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;
							message.Format("(v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
							message.Format("(v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
						}
						nobreak = false ;  break ;
					}
				}
				// Cas particuliers
				if ( MouvementNumero1 == true )  { MouvementNumero1 = false ;  NumeroMouvementPrecedent = 0 ;  }
				else if ( nobreak == true ) NumeroMouvementPrecedent = 1 ;	

				// On calcule NumeroMouvement pour le mvt actuel
				int NumeroMouvement = IncrementationComptageMouvements(NumeroMouvementPrecedent, -1, ptrsc) ;		
				if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement = %d", NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;  }
				
				// Tracés sur le graphe :
				// - sur Niveau :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, index, Niveau-2*Interligne, 9, orange, ptrsc) ;      
				SCString Texte ;  Texte.Format("Mvt %d\n", NumeroMouvement) ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;
				
				DrawText_UTAMAOA (Texte, index-1, Niveau-2*Interligne, 9, orange, DT_TOP | DT_RIGHT, ptrsc) ;
				// - sur PreviousHighest :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, PreviousHighest_Index, PreviousHighest+1*Interligne, 9, marron, ptrsc) ;       		
				if ( NumeroMouvement == 1 )  { Texte.Format("Debut Mvt 1\n") ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;  }  else { Texte.Format("Fin Mvt %d\n", NumeroMouvement-1) ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ; }
				DrawText_UTAMAOA (Texte, PreviousHighest_Index-1, PreviousHighest+1*Interligne, 9, marron, DT_BOTTOM | DT_RIGHT, ptrsc) ;
				
				// v_Mouvements : on crée un objet ObjetMvt qui contient les datas du mouvement actuel, puis cet objet est rajouté au vector v_Mouvements (note : v_Mouvements est un pointeur)
				StructureMouvement ObjetMvt ;
				ObjetMvt.	NumeroMouvement = NumeroMouvement ;	
				ObjetMvt.	TypeMouvement = -1 ;
				ObjetMvt.	Amplitude_pdm = AmplitudeActuelle_pdm ;
				ObjetMvt.	IndexCrossing = index ;
				ObjetMvt.	IndexExtremum = PreviousHighest_Index ;
				v_Mouvements->push_back(ObjetMvt) ;
				
				IndexPreviousCrossing = index ;
			}
			
			// RAZ des variables
			PreviousLowest = Niveau ;
			PreviousHighest = Niveau ;
			PreviousLowest_Index = index ;
			PreviousHighest_Index = index ;
		}

		// cas général 2 :  si on vient de dépasser Niveau : MVT HAUSSIER
		else if ( sc.High[index-1] < Niveau-Epsilon  and  sc.High[index] >= Niveau-Epsilon )
		{
			float AmplitudeActuelle_pdm = (Niveau-PreviousLowest)/Niveau*10000 ;
			if ( AmplitudeActuelle_pdm > MouvementRelatif_Seuil*10000  or  MouvementNumero1 )  			// le mvt est supérieur à x pdm  OU  c'est le premier passage
			{
				// si le mouvement précédent était indéterminé, on rectifie cette indétermination ici
				if ( TypeMouvementInitial == -2 )  
				{
					TypeMouvementInitial = -1 ;	// le mvt initial était baissier
					v_Mouvements->begin()->TypeMouvement = TypeMouvementInitial ;
				}
				
				// On itère sur les mvts précédents (en partant du dernier) tant que le mouvement actuel est plus important que les précédents  	
				int NumeroMouvementPrecedent = 999 ;
				std::vector<StructureMouvement>::iterator v_Mouvements_iterator = v_Mouvements->end() ;
				bool nobreak = true ;		// reste vrai si on breake pas durant la boucle while
				while ( v_Mouvements_iterator != v_Mouvements->begin() )
				{
					-- v_Mouvements_iterator ;
					if ( ( v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm )  or  ( v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm ) )	// le mvt actuel n'est pas plus important que ce mouvement précédent 
					{
						NumeroMouvementPrecedent = min(NumeroMouvementPrecedent, v_Mouvements_iterator->NumeroMouvement) ;		

						if ( DebugDansMessageLogONOFF_Input.GetYesNo() )
						{
							SCString message ;  
							message.Format("--------------------") ;  sc.AddMessageToLog(message,0) ;
							message.Format("AmplitudeActuelle_pdm = %5.0f pdm | NumeroMouvementPrecedent = %d", AmplitudeActuelle_pdm, NumeroMouvementPrecedent) ;  sc.AddMessageToLog(message,0) ;
							message.Format("v_Mouvements_iterator->Amplitude_pdm (important) = %5.0f | v_Mouvements_iterator->NumeroMouvement = %d", v_Mouvements_iterator->Amplitude_pdm, v_Mouvements_iterator->NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;
							message.Format("(v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
							message.Format("(v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
						}
						nobreak = false ;  break ;
					}
				}
				// Cas particulier
				if ( MouvementNumero1 == true )  { MouvementNumero1 = false ;  NumeroMouvementPrecedent = 0 ;  }
				else if ( nobreak == true ) NumeroMouvementPrecedent = 1 ;	

				// On calcule NumeroMouvement pour le mvt actuel
				int NumeroMouvement = IncrementationComptageMouvements(NumeroMouvementPrecedent, 1, ptrsc) ;		
				if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement = %d", NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;  }
				
				// Tracés sur le graphe
				// - sur Niveau :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, index, Niveau+2*Interligne, 9, orange, ptrsc) ;       				// void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
				SCString Texte ;  Texte.Append(Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm)) ;  Texte.AppendFormat("\nMvt %d", NumeroMouvement) ;
				DrawText_UTAMAOA (Texte, index-1, Niveau+2*Interligne, 9, orange, DT_BOTTOM | DT_RIGHT, ptrsc) ;
				// - sur PreviousLowest :
				DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, PreviousLowest_Index, PreviousLowest-1*Interligne, 9, marron, ptrsc) ;       		
				if ( NumeroMouvement == 1 )  { Texte = Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm) ;  Texte.AppendFormat("\nDebut Mvt 1") ;  }  else { Texte = Conversion_Amplitude_pdm_En_SCString(AmplitudeActuelle_pdm) ; Texte.AppendFormat("\nFin Mvt %d", NumeroMouvement-1) ;  }
				DrawText_UTAMAOA (Texte, PreviousLowest_Index-1, PreviousLowest-1*Interligne, 9, marron, DT_TOP | DT_RIGHT, ptrsc) ;
		
				// v_Mouvements
				StructureMouvement ObjetMvt ;
				ObjetMvt.	NumeroMouvement = NumeroMouvement ;	
				ObjetMvt.	TypeMouvement = 1 ;
				ObjetMvt.	Amplitude_pdm = AmplitudeActuelle_pdm ;
				ObjetMvt.	IndexCrossing = index ;
				ObjetMvt.	IndexExtremum = PreviousLowest_Index ;
				v_Mouvements->push_back(ObjetMvt) ;
				
				IndexPreviousCrossing = index ;
			}
			
			// RAZ des variables
			PreviousLowest = Niveau ;
			PreviousHighest = Niveau ;
			PreviousLowest_Index = index ;
			PreviousHighest_Index = index ;			
		}

		// Variables recalculées sur chaque bougie
		if ( index > IndexPreviousCrossing )			// l'index doit être supérieur à l'index du crossing de précédent mouvement
		{
			if ( sc.Low[index] < PreviousLowest )
			{
				PreviousLowest = sc.Low[index] ;
				PreviousLowest_Index = index ;
			}
			if ( sc.High[index] > PreviousHighest )
			{
				PreviousHighest = sc.High[index] ;
				PreviousHighest_Index = index ;
			}
		}
	}
	

	// A chaque appel du DLL on réévalue NumeroMouvement_EnCours si Affichage est ON
	if ( AffichageONOFF_Input.GetYesNo() == 0 )
	{
		// Zeroing tools
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// SetButton22_NumeroMouvement et Set_Button_18_Amplitude_And_Button_19_Bonus
		SetButton22_NumeroMouvement ("Affichage OFF", 0, ptrsc) ;
		Set_Button_18_Amplitude_And_Button_19_Bonus ("Affichage OFF", 0, 0, 0, 0, ptrsc) ;	
	}
	else if ( Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine == false )
	{
		// scan rapide du vector "v_Mouvements" pour déterminer si Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine
		{
			// on parcourt le vector "v_Mouvements" entre begin et end pour trouver un IndexCrossing supérieur à IndexHoraireRAZ => le mvt correspond au mvt backtesté (ou au mvt tradé, s'il n'a pas été trouvé)
			std::vector<StructureMouvement>::iterator v_Mouvements_iterator = v_Mouvements->begin() ;
			bool nobreak = true ;		// reste vrai si on breake pas durant la boucle while
			while ( v_Mouvements_iterator != v_Mouvements->end() )			// si la boucle while est interrompue au test initial, ça veut dire qu'on est sur le mvt 1
			{
				if ( v_Mouvements_iterator->IndexCrossing >= IndexHoraireRAZ )	// le mvt succède à IndexHoraireRAZ
				{
					int NumeroMouvement_EnCours = v_Mouvements_iterator->NumeroMouvement ;		 
					float Amplitude_pdm_EnCours = v_Mouvements_iterator->Amplitude_pdm ;		 
					float Position_EnCours = - v_Mouvements_iterator->TypeMouvement ;		 
					int IndexExtremum = v_Mouvements_iterator->IndexExtremum ;
					float Amplitude_pts_EnCours = Amplitude_pdm_EnCours * Niveau / 10000 ;
					float Extremum = Niveau + Position_EnCours * Amplitude_pts_EnCours ;

					// SetButton22_NumeroMouvement et Set_Button_18_Amplitude_And_Button_19_Bonus
					SetButton22_NumeroMouvement ("", NumeroMouvement_EnCours, ptrsc) ;
					float Bonus_PlanA, Bonus_PlanB ;
					float RatioAmplitude_PlanA = Amplitude_pts_EnCours / RangePrecedent_Input.GetFloat() ;
					CalculDuBonus(NatureNiveau_Input.GetInt(), SousNatureNiveau_Input.GetInt(), NumeroMouvement_EnCours, Amplitude_pdm_EnCours, RatioAmplitude_PlanA, ptrsc, Bonus_PlanA, Bonus_PlanB) ;		// void CalculDuBonus(int NatureNiveau, int SousNatureNiveau, int NumeroMouvement, float Amplitude_pdm, float RatioAmplitude_PlanA, s_sc* ptrsc, float & Bonus_PlanA, float & Bonus_PlanB)	
					Set_Button_18_Amplitude_And_Button_19_Bonus ("", Amplitude_pdm_EnCours, RatioAmplitude_PlanA, Bonus_PlanA, Bonus_PlanB, ptrsc) ;		
					
					// Drawings sur le chart
					DrawSegmentPointille_UTAMAOA (0, (Niveau+Extremum)/2, IndexHoraireRAZ, (Niveau+Extremum)/2, 2, RGB(0,127,255), ptrsc, 1000003) ;  // segment tracé à mi-chemin entre Niveau et Extremum
					DrawSegmentPointille_UTAMAOA (0, Niveau-Position_EnCours*Amplitude_pts_EnCours, IndexHoraireRAZ, Niveau-Position_EnCours*Amplitude_pts_EnCours, 2, RGB(0,127,255), ptrsc, 1000004) ;  // segment tracé en symétrique à Niveau

					// Sortie de la boucle while
					Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine = true ;  break ;
				}
				else ++ v_Mouvements_iterator ;
			}
		}
		
		if ( Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine == false )			// cas typique du trading en temps réel
		{
			if ( LastIndex < IndexFinPeriode )			
			{
				if ( sc.Low[LastIndex] >= Niveau+Epsilon  )			// Mvt baissier
				{
					float AmplitudeActuelle_pdm = (PreviousHighest-Niveau)/Niveau*10000 ;
					if ( AmplitudeActuelle_pdm > MouvementRelatif_Seuil*10000 )  // le mvt est supérieur à x pdm
					{
						// Pour trouver NumeroMouvementPrecedent, on itère sur les mvts précédents (en partant du dernier) tant que le mouvement actuel est plus important que les précédents  		
						int NumeroMouvementPrecedent = 999 ;
						std::vector<StructureMouvement>::iterator v_Mouvements_iterator = v_Mouvements->end() ;
						bool nobreak = true ;		// reste vrai si on ne breake pas durant la boucle while
						while ( v_Mouvements_iterator != v_Mouvements->begin() )			// pour le mouvement 1, v_Mouvements est de taille 0, donc la boucle while est interrompue au test initial
						{
							-- v_Mouvements_iterator ;
							if ( ( v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm )  or  ( v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm ) )	// le mvt actuel n'est pas plus important que ce mouvement précédent 
							{
								NumeroMouvementPrecedent = min(NumeroMouvementPrecedent, v_Mouvements_iterator->NumeroMouvement) ;		
								
								if ( DebugDansMessageLogONOFF_Input.GetYesNo() )
								{
									SCString message ;  
									message.Format("--------------------") ;  sc.AddMessageToLog(message,0) ;
									message.Format("AmplitudeActuelle_pdm = %5.0f pdm | NumeroMouvementPrecedent = %d", AmplitudeActuelle_pdm, NumeroMouvementPrecedent) ;  sc.AddMessageToLog(message,0) ;
									message.Format("v_Mouvements_iterator->Amplitude_pdm (important) = %5.0f | v_Mouvements_iterator->NumeroMouvement = %d", v_Mouvements_iterator->Amplitude_pdm, v_Mouvements_iterator->NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;
									message.Format("(v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
									message.Format("(v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
								}
								nobreak = false ;  break ;
							}
						}
						// Cas particuliers
						if ( MouvementNumero1 == true )  { /* MouvementNumero1 = false ;   */  NumeroMouvementPrecedent = 0 ;  }			// ici, il ne faut pas modifier le booléen MouvementNumero1
						else if ( nobreak == true ) NumeroMouvementPrecedent = 1 ;	

						// On calcule NumeroMouvement_EnCours pour le mvt actuel
						int NumeroMouvement_EnCours = IncrementationComptageMouvements(NumeroMouvementPrecedent, -1, ptrsc) ;		
						if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement_EnCours = %d", NumeroMouvement_EnCours) ;  sc.AddMessageToLog(message,0) ;  }
						
						// SetButton22_NumeroMouvement
						SetButton22_NumeroMouvement ("", NumeroMouvement_EnCours, ptrsc) ;
						
						// Set_Button_18_Amplitude_And_Button_19_Bonus
						float Amplitude_pts_EnCours = PreviousHighest-Niveau ;
						float Bonus_PlanA, Bonus_PlanB ;
						float RatioAmplitude_PlanA = Amplitude_pts_EnCours / RangePrecedent_Input.GetFloat() ;
						CalculDuBonus(NatureNiveau_Input.GetInt(), SousNatureNiveau_Input.GetInt(), NumeroMouvement_EnCours, AmplitudeActuelle_pdm, RatioAmplitude_PlanA, ptrsc, Bonus_PlanA, Bonus_PlanB) ;		// void CalculDuBonus(int NatureNiveau, int SousNatureNiveau, int NumeroMouvement, float Amplitude_pdm, float RatioAmplitude_PlanA, s_sc* ptrsc, float & Bonus_PlanA, float & Bonus_PlanB)	
						Set_Button_18_Amplitude_And_Button_19_Bonus ("", AmplitudeActuelle_pdm, RatioAmplitude_PlanA, Bonus_PlanA, Bonus_PlanB, ptrsc) ;		

						// Drawings sur le chart
						int Position_EnCours = 1 ; 		// Mvt baissier
						float Extremum = Niveau + Position_EnCours * Amplitude_pts_EnCours ;
						int IndexExtremum = PreviousHighest_Index ;
						// DrawTextAjusteDroite_UTAMAOA (Texte_Amplitude_Bonus, IndexExtremum-1, Extremum+1.6*Position_EnCours*Interligne, 11, RGB(0,200,0), ptrsc, 1000001) ;
						DrawSegmentPointille_UTAMAOA (0, (Niveau+Extremum)/2, IndexHoraireRAZ, (Niveau+Extremum)/2, 2, RGB(0,127,255), ptrsc, 1000003) ;  // segment tracé à mi-chemin entre Niveau et Extremum
						DrawSegmentPointille_UTAMAOA (0, Niveau-Position_EnCours*Amplitude_pts_EnCours, IndexHoraireRAZ, Niveau-Position_EnCours*Amplitude_pts_EnCours, 2, RGB(0,127,255), ptrsc, 1000004) ;  // segment tracé en symétrique à Niveau
					}
					else
					{
						SetButton22_NumeroMouvement ("Next Mvt n.d.", 0, ptrsc) ;
						Set_Button_18_Amplitude_And_Button_19_Bonus ("Next Mvt n.d.", 0, 0, 0, 0, ptrsc) ;
					}
				}
				else if ( sc.High[LastIndex] <= Niveau-Epsilon )			// Mvt haussier
				{
					float AmplitudeActuelle_pdm = (Niveau-PreviousLowest)/Niveau*10000 ;
					if ( AmplitudeActuelle_pdm > MouvementRelatif_Seuil*10000 )  // le mvt est supérieur à x pdm
					{
						// On itère sur les mvts précédents (en partant du dernier) tant que le mouvement actuel est plus important que les précédents  	
						int NumeroMouvementPrecedent = 999 ;
						std::vector<StructureMouvement>::iterator v_Mouvements_iterator = v_Mouvements->end() ;
						bool nobreak = true ;		// reste vrai si on breake pas durant la boucle while
						while ( v_Mouvements_iterator != v_Mouvements->begin() )
						{
							-- v_Mouvements_iterator ;
							if ( ( v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm )  or  ( v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm ) )	// le mvt actuel n'est pas plus important que ce mouvement précédent 
							{
								NumeroMouvementPrecedent = min(NumeroMouvementPrecedent, v_Mouvements_iterator->NumeroMouvement) ;		

								if ( DebugDansMessageLogONOFF_Input.GetYesNo() )
								{
									SCString message ;  
									message.Format("--------------------") ;  sc.AddMessageToLog(message,0) ;
									message.Format("AmplitudeActuelle_pdm = %5.0f pdm | NumeroMouvementPrecedent = %d", AmplitudeActuelle_pdm, NumeroMouvementPrecedent) ;  sc.AddMessageToLog(message,0) ;
									message.Format("v_Mouvements_iterator->Amplitude_pdm (important) = %5.0f | v_Mouvements_iterator->NumeroMouvement = %d", v_Mouvements_iterator->Amplitude_pdm, v_Mouvements_iterator->NumeroMouvement) ;  sc.AddMessageToLog(message,0) ;
									message.Format("(v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == 1 and  AmplitudeActuelle_pdm < 2*v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
									message.Format("(v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) = %d", v_Mouvements_iterator->TypeMouvement == -1 and  AmplitudeActuelle_pdm < v_Mouvements_iterator->Amplitude_pdm) ;  sc.AddMessageToLog(message,0) ;
								}
								nobreak = false ;  break ;
							}
						}
						// Cas particuliers
						if ( MouvementNumero1 == true )  { /* MouvementNumero1 = false ;   */ NumeroMouvementPrecedent = 0 ;  }
						else if ( nobreak == true ) NumeroMouvementPrecedent = 1 ;	

						// On calcule NumeroMouvement_EnCours pour le mvt actuel
						int NumeroMouvement_EnCours = IncrementationComptageMouvements(NumeroMouvementPrecedent, 1, ptrsc) ;		
						if ( DebugDansMessageLogONOFF_Input.GetYesNo() )  { SCString message ;  message.Format("NumeroMouvement_EnCours = %d", NumeroMouvement_EnCours) ;  sc.AddMessageToLog(message,0) ;  }
						
						// SetButton22_NumeroMouvement
						SetButton22_NumeroMouvement ("", NumeroMouvement_EnCours, ptrsc) ;
						
						// Set_Button_18_Amplitude_And_Button_19_Bonus
						float Amplitude_pts_EnCours = Niveau-PreviousLowest ;
						float Bonus_PlanA, Bonus_PlanB ;
						float RatioAmplitude_PlanA = Amplitude_pts_EnCours / RangePrecedent_Input.GetFloat() ;
						CalculDuBonus(NatureNiveau_Input.GetInt(), SousNatureNiveau_Input.GetInt(), NumeroMouvement_EnCours, AmplitudeActuelle_pdm, RatioAmplitude_PlanA, ptrsc, Bonus_PlanA, Bonus_PlanB) ;		// void CalculDuBonus(int NatureNiveau, int SousNatureNiveau, int NumeroMouvement, float Amplitude_pdm, float RatioAmplitude_PlanA, s_sc* ptrsc, float & Bonus_PlanA, float & Bonus_PlanB)	
						Set_Button_18_Amplitude_And_Button_19_Bonus ("", AmplitudeActuelle_pdm, RatioAmplitude_PlanA, Bonus_PlanA, Bonus_PlanB, ptrsc) ;		
						
						// Drawings sur le chart
						int Position_EnCours = -1 ; 		// Mvt haussier
						float Extremum = Niveau + Position_EnCours * Amplitude_pts_EnCours ;
						int IndexExtremum = PreviousLowest_Index ;
						// DrawTextAjusteDroite_UTAMAOA (Texte_Amplitude_Bonus, IndexExtremum-1, Extremum+1.6*Position_EnCours*Interligne, 11, RGB(0,200,0), ptrsc, 1000001) ;
						DrawSegmentPointille_UTAMAOA (0, (Niveau+Extremum)/2, IndexHoraireRAZ, (Niveau+Extremum)/2, 2, RGB(0,127,255), ptrsc, 1000003) ;  // segment tracé à mi-chemin entre Niveau et Extremum
						DrawSegmentPointille_UTAMAOA (0, Niveau-Position_EnCours*Amplitude_pts_EnCours, IndexHoraireRAZ, Niveau-Position_EnCours*Amplitude_pts_EnCours, 2, RGB(0,127,255), ptrsc, 1000004) ;  // segment tracé en symétrique à Niveau
					}		
					else
					{
						SetButton22_NumeroMouvement ("Next Mvt n.d.", 0, ptrsc) ;
						Set_Button_18_Amplitude_And_Button_19_Bonus ("Next Mvt n.d.", 0, 0, 0, 0, ptrsc) ;
					}
				}
				else
				{
					SetButton22_NumeroMouvement ("Next Mvt n.d.", 0, ptrsc) ;
					Set_Button_18_Amplitude_And_Button_19_Bonus ("Next Mvt n.d.", 0, 0, 0, 0, ptrsc) ;
				}
			}
			else
			{
				Bonus_Plans_A_et_B_Input.SetFloat(-1) ; 
				SetButton22_NumeroMouvement ("> Fin Periode", 0, ptrsc) ;
				Set_Button_18_Amplitude_And_Button_19_Bonus ("> Fin Periode", 0, 0, 0, 0, ptrsc) ;	
				Le_Mouvement_Apres_IndexHoraireRAZ_Est_Termine = true ; 	
			}
		}
	}
	
	
	// Status Report (CTRL+S)
	if (sc.MenuEventID == ACS_BUTTON_3 )
	{
		SCString message ;
		
		// Première ligne
		message.Format("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS | STATUS REPORT | SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS") ;  sc.AddMessageToLog(message,0) ;
		// message.Format("Numero         Type      Amplitude    IndexCrossing   ") ;  sc.AddMessageToLog(message,0) ;

		// Contenu du vector v_Mouvements
		for (auto v_Mouvements_iterator  : * v_Mouvements )			// les contenus du vecteur pointé par v_Mouvements sont parcourus du premier au dernier
		{
			message.Format("Mvt %2d          %2d          %5.0f pdm          %5d          ", v_Mouvements_iterator.NumeroMouvement, v_Mouvements_iterator.TypeMouvement, v_Mouvements_iterator.Amplitude_pdm, v_Mouvements_iterator.	IndexCrossing) ;  sc.AddMessageToLog(message,0) ;
		}
		
		// Dernière ligne
		message.Format("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS | END STATUS REPORT | SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS") ;  sc.AddMessageToLog(message,0) ;
	}
	
	
	
	// lorsque je suis déconnecté de la data, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  sc.UpdateAlways = 1 ;


	
	if(sc.LastCallToFunction)
	{
		// Libération de la mémoire occupée par v_Mouvements
		if (v_Mouvements != NULL)
		{
			delete v_Mouvements;
			sc.SetPersistentPointer(2, NULL);
		}
		
		sc.SetCustomStudyControlBarButtonText(18, "      ") ; 
		sc.SetCustomStudyControlBarButtonText(19, "      ") ; 
		sc.SetCustomStudyControlBarButtonText(22, "      ") ; 
	}
	
	
	
	
}



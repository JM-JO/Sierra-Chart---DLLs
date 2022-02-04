/* 
v1.0 (2019/2/8) : cet indicateur retourne les VWAP jour, hebdo et mensuel. Il est identique à l'indicateur "VWAP calcul v1.3.4" de PRT.
v1.1 (2019/2/9) : dans le même DLL on rajoute une study qui récupère les VWAPs sur les graphes 200/2000-ticks pour les afficher sur le 16-ticks.
v1.1.1 (2019/2/11) : récupération des VWAP J/H/M sur les graphes 200/2000-ticks pour les afficher sur le 16-ticks.
v1.3 (2019/2/18) : correction de bug lié à  l'emploi de variables persistantes (le DLL pouvant être exécuté plus d'une fois par barre lors du trading en live). Les incrémentations ne sont réalisées qu'une fois par barre.
v1.4 (2019/2/18) : exclusion des jours sans settlement.
v1.5 (2019/2/25) : la study "affichage" récupère la StudyID et le SubGraph de la study "calcul" source par le biais du Name du SubGraph.
v1.6 (2019/7/4) : le tracé des VWAPs est togglé ON/OFF via un bouton. Le statut est géré par la study "Prix typique". La fonction scsf_VWAPsAffichage consulte cette study. Compatible v1941.
v1.7 (2019/10/27) : tracé des PP en vectoriel. Le niveau retenu est celui de index = sc.LastVisibleBar.



Améliorations à apporter :


SOMMAIRE DES FONCTIONS :
BooleanHideStudy
scsf_VWAPsCalcul
scsf_VWAPsAffichage


 */

 
 
#include "sierrachart.h"


SCDLLName("VWAPs JHM")



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int BooleanHideStudy(s_sc* ptrsc)
{
	int ShowStudy = 1 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''Prix typique''. Necessaire pour determiner sc.HideStudy.") ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique, 3, ShowStudy) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return (1 - ShowStudy) ;    // HideStudy = 1 - ShowStudy
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
struct MonTexteString : s_UseTool // MonTexteString crée du texte avec un string
{											
	MonTexteString(SCString Texte, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_TEXT;
		//BeginDateTime = 0;  // 0 = position à gauche ; 150 pour la position à droite
		//TextAlignment = DT_VCENTER | DT_LEFT ;
		BeginDateTime = 150;  // 0 = position à gauche ; 150 pour la position à droite
		TextAlignment = DT_VCENTER | DT_RIGHT ;
		//FontBold = 1;
		Color = RGB(220, 220, 220); //background gris du graphique
		BeginValue = niveau;
		FontSize = epaisseur+6;
		FontBackColor = rgb;
		Text = Texte;			
		Region = ptrsc->GraphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigneJusquAIndex : s_UseTool  // MaLigneJusquALastIndex crée une ligne entre index=beginIndex et index=endIndex
	{
		MaLigneJusquAIndex(int beginIndex, int endIndex, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginIndex = beginIndex ;
			EndIndex = endIndex ;
			BeginValue = niveau ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = ptrsc->GraphRegion ; 
		} 
	};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		if ( monEpaisseur > 0 )
		{
			//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
			MaLigneJusquAIndex objetMaLigne(beginIndex, endIndex, niveau, monEpaisseur, maCouleur, ptrsc);
			ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
			//Création d'instances de MonTexte et appel de sc.UseTool
			MonTexteString objetMonTexteString(Texte, niveau, monEpaisseur, maCouleur, ptrsc);
			ptrsc->UseTool(objetMonTexteString); 
		}
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_VWAPsCalcul(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef AffichageVWAPJ = sc.Subgraph[1]; 
	SCSubgraphRef AffichageVWAPH = sc.Subgraph[2];
	SCSubgraphRef AffichageVWAPM = sc.Subgraph[3]; 	
	//SCSubgraphRef TraceDirecteVWAPH = sc.Subgraph[3]; 	
	
	double& SommePrixPonderesJ = sc.GetPersistentDouble(1);
	double& SommePrixPonderesH = sc.GetPersistentDouble(2);
	double& SommePrixPonderesM = sc.GetPersistentDouble(3);
	int& SommeVolumeJ = sc.GetPersistentInt(4);
	int& SommeVolumeH = sc.GetPersistentInt(5);
	int& SommeVolumeM = sc.GetPersistentInt(6);
	int& IndexChandelierComplet = sc.GetPersistentInt(7);  // Incrémentations uniquement sur des chandeliers complétés. Lors de l'appel de la fonction scsf, le précédent chandelier complet correspond à IndexChandelierComplet.
	int Year, Month, Day;
	sc.BaseDateTimeIn[sc.Index].GetDateYMD(Year, Month, Day);
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "VWAPs calculs JHM"; 
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;
		
		
		AffichageVWAPJ.Name = "VWAP J"; 
		AffichageVWAPJ.ShortName = "VWAP J"; 
		AffichageVWAPH.Name = "VWAP H"; 
		AffichageVWAPH.ShortName = "VWAP H"; 
		AffichageVWAPM.Name = "VWAP M";
		AffichageVWAPM.ShortName = "VWAP M";		
		//TraceDirecteVWAPH.Name = "TraceDirecteVWAPH"; 
		
		AffichageVWAPJ.LineWidth = 1;
		AffichageVWAPH.LineWidth = 1;
		AffichageVWAPM.LineWidth = 1;
		//TraceDirecteVWAPH.LineWidth = 1;
		
		AffichageVWAPJ.PrimaryColor = RGB(176,176,0);   
		AffichageVWAPH.PrimaryColor = RGB(130,130,130);
		AffichageVWAPM.PrimaryColor = RGB(0,0,0);		
		//TraceDirecteVWAPH.PrimaryColor = RGB(0,0,0);		
		
		for (int SubgraphIndex = 1; SubgraphIndex <= 3; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			//sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false; 
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
		}
		
	return ;	
	}
	
	
	
	if ( sc.Index== 0 )   // sc.Index==0 est toujours un début de séance
	{
		int xTicks ; 
		bool enTicks ;
		
		// détermination du XTicks (ne fonctionne pas si placé dans sc.SetDefaults)
		n_ACSIL :: s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )   // on vérifie que le graphe est bien en XTicks
		{	
			enTicks = 1 ;
			xTicks = BarPeriod.IntradayChartBarPeriodParameter1 ; 	
			if  (! ( xTicks == 200 or xTicks==2000 ) )   // si le graphe n'est pas en 200-Ticks ou en 2000-ticks => message d'erreur
			{	
				SCString message ;   
				message.Format("!! ERREUR !! Le graphique n'est pas en 200-ticks ou en 2000-ticks.") ;    
				sc.AddMessageToLog(message,1) ;
			}  
		}
		else // message d'erreur dans le log si on n'est pas en ticks
		{
			enTicks = 0 ;
			SCString message ;   
			message.Format("!! ERREUR !! Le graphique n'est pas en ticks.") ;    
			sc.AddMessageToLog(message,1) ;
		}	
		
		// création des attributs Name
		if (enTicks) ;
		{
			if (xTicks == 200)
			{  AffichageVWAPJ.Name = "VWAP J 200-ticks" ;  AffichageVWAPH.Name = "VWAP H 200-ticks" ; AffichageVWAPM.Name = "VWAP M 200-ticks" ; }
			if (xTicks == 2000)
			{  AffichageVWAPJ.Name = "VWAP J 2000-ticks" ;  AffichageVWAPH.Name = "VWAP H 2000-ticks" ; AffichageVWAPM.Name = "VWAP M 2000-ticks" ; }
		}
		
		
		// initialisation des variables journalières
		AffichageVWAPJ[sc.Index] = 0 ;  SommePrixPonderesJ = 0 ;  SommeVolumeJ = 0;
		
		// initialisation des variables hebdo
		if (  sc.BaseDateTimeIn[sc.Index].GetDayOfWeek()  == 0  ) // si la séance débute un dimanche, on peut débuter ici le calcul du VWAPH
				{	AffichageVWAPH[sc.Index] = 0 ;  SommePrixPonderesH = 0 ;  SommeVolumeH = 0 ;  }
		else 	{   AffichageVWAPH[sc.Index] = 0 ; SommePrixPonderesH = 0 ; SommeVolumeH = -1000000000 ; }
		
		// initialisation des variables mensuelles
		if (  	(  Month==1 and Day==31 )  or (  Month==2 and Day==28 )  or (  Month==3 and Day==31 )  or (  Month==4 and Day==30 )  or (  Month==5 and Day==31 )  or (  Month==6 and Day==30 )  or (  Month==7 and Day==31 )  or 
				(  Month==8 and Day==31 )  or (  Month==9 and Day==30 )  or (  Month==10 and Day==31 )  or (  Month==11 and Day==30 )  or (  Month==1 and Day==31 )   ) // si la séance débute le dernier jour du mois, on peut débuter ici le calcul du VWAPM
				{	AffichageVWAPM[sc.Index] = 0 ;  SommePrixPonderesM = 0 ;  SommeVolumeM = 0 ;  }
		else 	{   AffichageVWAPM[sc.Index] = 0 ; SommePrixPonderesM = 0 ; SommeVolumeM = -1000000000 ; }
	
		// initialisation de IndexChandelierComplet
		IndexChandelierComplet = 0 ;
	}
	

	// Incrémentations lorsqu'on arrive sur un nouveau chandelier
	if (sc.Index > IndexChandelierComplet)
	{
		SommePrixPonderesJ += ( sc.High[sc.Index-1] + sc.Low[sc.Index-1] ) / 2 * sc.Volume[sc.Index-1] ; 
		SommeVolumeJ += sc.Volume[sc.Index-1] ;
		SommePrixPonderesH += ( sc.High[sc.Index-1] + sc.Low[sc.Index-1] ) / 2 * sc.Volume[sc.Index-1] ; 
		SommeVolumeH += sc.Volume[sc.Index-1] ;
		SommePrixPonderesM += ( sc.High[sc.Index-1] + sc.Low[sc.Index-1] ) / 2 * sc.Volume[sc.Index-1] ; 
		SommeVolumeM += sc.Volume[sc.Index-1] ;	
		IndexChandelierComplet ++ ;
	}

	// Changement de séance : pour calcul VWAPJ et VWAPM
	if (  sc.BaseDateTimeIn.TimeAt(sc.Index) == 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)!= 64800   and   SommeVolumeJ != 0      //64800 est le nb de secondes à 18h    //   " and SommeVolumeJ != 0 " car si le chandelier de 18h00m00s est calculé deux fois par scsf, SommeVolumeJ vaudra 0.
			and   (  not  (   ( Year==2019 and Month==2 and Day==18)  or  ( Year==2019 and Month==1 and Day==21)   )   )   )   // exclusion des journées sans settlement
	
	
	{
		
		// Pour le VWAPJ
		AffichageVWAPJ[sc.Index] = SommePrixPonderesJ / SommeVolumeJ ;
		SommePrixPonderesJ = 0 ;
		SommeVolumeJ = 0 ;
		
		// Pour le VWAPM
		// Changement de mois :   ( on change de séance càd il est 18h)  ET   (  (on est le dernier jour du mois) OU ( le mois a changé entre deux chandeliers successifs càd on est le WE )  )
		if (    (  Month==1 and Day==31 )  or (  Month==2 and Day==28 )  or (  Month==3 and Day==31 )  or (  Month==4 and Day==30 )  or (  Month==5 and Day==31 )  or (  Month==6 and Day==30 )  or (  Month==7 and Day==31 )  or 
		(  Month==8 and Day==31 )  or (  Month==9 and Day==30 )  or (  Month==10 and Day==31 )  or (  Month==11 and Day==30 )  or (  Month==1 and Day==31 )  or (  sc.BaseDateTimeIn[sc.Index].GetMonth()  !=   sc.BaseDateTimeIn[sc.Index-1].GetMonth()  )   )
		{
			AffichageVWAPM[sc.Index] = SommePrixPonderesM / SommeVolumeM ;
			if ( SommeVolumeM < 0 )  {  AffichageVWAPM[sc.Index] = 0 ;  }    // la première semaine n'était pas complète
			SommePrixPonderesM = 0 ;
			SommeVolumeM = 0;			
		}
		else  // changement de séance sans changement de mois
		{ AffichageVWAPM[sc.Index] = AffichageVWAPM[sc.Index-1] ;}
		
	}
	
	else  // on ne change pas de séance
	{ AffichageVWAPJ[sc.Index] = AffichageVWAPJ[sc.Index-1] ;  AffichageVWAPM[sc.Index] = AffichageVWAPM[sc.Index-1] ;}


	
	// Changement de semaine : pour calcul VWAPH
	 if ( sc.BaseDateTimeIn[sc.Index].GetDayOfWeek()   <   sc.BaseDateTimeIn[sc.Index-1].GetDayOfWeek()  and  SommeVolumeH != 0  )  
	 {
		AffichageVWAPH[sc.Index] = SommePrixPonderesH / SommeVolumeH ;
		if ( SommeVolumeH < 0 )  {  AffichageVWAPH[sc.Index] = 0 ;  }    // la première semaine n'était pas complète
		SommePrixPonderesH = 0 ;
		SommeVolumeH = 0;
	 }
	else
	{	AffichageVWAPH[sc.Index] = AffichageVWAPH[sc.Index-1] ;	 }
	
	

	
	//TraceDirecteVWAPH[sc.Index] = SommePrixPonderesH / SommeVolumeH ;
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_VWAPsAffichage(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef AffichageVWAPJ = sc.Subgraph[1];    // appelé par AMR
	SCSubgraphRef AffichageVWAPH = sc.Subgraph[2];   // appelé par AMR
	SCSubgraphRef AffichageVWAPM = sc.Subgraph[3];  // appelé par AMR
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  
	
	
	SCInputRef ChartNumber200ticks = sc.Input[1]; 
	SCInputRef ChartNumber2000ticks = sc.Input[2]; 
	SCInputRef TraceVectorielONOFF = sc.Input[3];
	
	SCInputRef ChartInputsVWAPJ = sc.Input[11]; 
	SCInputRef ChartInputsVWAPH = sc.Input[12]; 
	SCInputRef ChartInputsVWAPM = sc.Input[13]; 



	if (sc.SetDefaults)
	{
		sc.GraphName = "VWAPs affichage JHM";    // appelé par AMR
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;
		
	
		ChartNumber200ticks.Name = "ChartNumber du 200-ticks" ;
		ChartNumber200ticks.SetChartNumber(11); 
		ChartNumber2000ticks.Name = "ChartNumber du 2000-ticks" ;
		ChartNumber2000ticks.SetChartNumber(12); 
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		
		
		AffichageVWAPJ.Name = "VWAP J"; 
		AffichageVWAPH.Name = "VWAP H"; 
		AffichageVWAPM.Name = "VWAP M"; 

		AffichageVWAPJ.LineWidth = 1;
		AffichageVWAPH.LineWidth = 1;
		AffichageVWAPM.LineWidth = 1;

		AffichageVWAPJ.PrimaryColor = RGB(176,176,0); 
		AffichageVWAPH.PrimaryColor = RGB(130,130,130);
		AffichageVWAPM.PrimaryColor = RGB(0,0,0);		

		for (int SubgraphIndex = 1; SubgraphIndex <= 3; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
		}
		
		return ;	
	}
	
	
	if ( sc.UpdateStartIndex == 0 )  // on crée les ChartStudySubgraphValues
	{
		sc.GraphRegion = 1; 

		// Récupération de la studyID sur le 200-ticks
		int StudyID = sc.GetStudyIDByName(ChartNumber200ticks.GetChartNumber(), "VWAPs calculs JHM", 0) ;
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source ne contient pas de calcul des VWAPs Jour et Hebdo") ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues pour VWAPJ
			ChartInputsVWAPJ.SetChartStudySubgraphValues(ChartNumber200ticks.GetChartNumber(),StudyID,1);
			// Création des ChartStudySubgraphValues pour VWAPH
			ChartInputsVWAPH.SetChartStudySubgraphValues(ChartNumber200ticks.GetChartNumber(),StudyID,2);
		}

		// Récupération de la studyID sur le 2000-ticks
		StudyID = sc.GetStudyIDByName(ChartNumber2000ticks.GetChartNumber(), "VWAPs calculs JHM", 0) ;
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source ne contient pas de calcul des VWAPs Mensuels") ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues pour VWAPM
			ChartInputsVWAPM.SetChartStudySubgraphValues(ChartNumber2000ticks.GetChartNumber(),StudyID,3);
		}
	}
	

for (int index = sc.UpdateStartIndex ; index < sc.ArraySize ; index++)                 // début AutoLoop   // REMPLACER LES index par index
	{
	
		// Changement de séance
		if (  (  sc.BaseDateTimeIn.TimeAt(index) == 64800  and  sc.BaseDateTimeIn.TimeAt(index-1)!= 64800  )   or  index== 0  )    //64800 est le nb de secondes à 18h 
		{	
			FirstIndex[index] = index ;
			
			// on récupère l'index du nouveau chandelier sur le chart d'où provient le VWAPJ (200 ticks)
			int ChartVWAPJTicksIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber200ticks.GetChartNumber(), index); 
			// on récupère la valeur du VWAPJ dans le subgraphe adéquat
			SCFloatArray SubGraphVWAPJ;
			sc.GetStudyArrayFromChartUsingID(ChartInputsVWAPJ.GetChartStudySubgraphValues(), SubGraphVWAPJ);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);
			AffichageVWAPJ[index] = SubGraphVWAPJ[ChartVWAPJTicksIndex] ;
			
			// on récupère l'index du nouveau chandelier sur le chart d'où provient le VWAPH (200 ticks)
			int ChartVWAPHTicksIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber200ticks.GetChartNumber(), index); 
			// on récupère la valeur du VWAPH dans le subgraphe adéquat
			SCFloatArray SubGraphVWAPH;
			sc.GetStudyArrayFromChartUsingID(ChartInputsVWAPH.GetChartStudySubgraphValues(), SubGraphVWAPH);  
			AffichageVWAPH[index] = SubGraphVWAPH[ChartVWAPHTicksIndex] ;
		
			// on récupère l'index du nouveau chandelier sur le chart d'où provient le VWAPM (2000 ticks)
			int ChartVWAPMTicksIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber2000ticks.GetChartNumber(), index); 
			// on récupère la valeur du VWAPM dans le subgraphe adéquat
			SCFloatArray SubGraphVWAPM;
			sc.GetStudyArrayFromChartUsingID(ChartInputsVWAPM.GetChartStudySubgraphValues(), SubGraphVWAPM); 
			AffichageVWAPM[index] = SubGraphVWAPM[ChartVWAPMTicksIndex] ;
		}
		else
		{ FirstIndex[index] = FirstIndex[index-1] ;  AffichageVWAPJ[index] = AffichageVWAPJ[index-1] ;  AffichageVWAPH[index] = AffichageVWAPH[index-1] ;  AffichageVWAPM[index] = AffichageVWAPM[index-1] ;  }
	
	}




	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
	}
	
	
	
	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie de la séance      // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 1; SubgraphIndex <= 3; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}	
	

}





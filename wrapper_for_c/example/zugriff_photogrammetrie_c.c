/*
  ============================================================================
  Name        : zugriff_photogrammetrie_c.c
  Author      : Steffen Scheller
  Version     :
  Copyright   : ©2010
  Description : C Zugriff auf Photogrammetrie (*.so,*.dll) C, Ansi-style
  ============================================================================
*/

#include <stdio.h>

//for windows -> every time extra conventions :-(
//#if defined(_MSC_VER)
//#include <windows.h>
//#endif

//c_wrapper to the photogrammetrie
#include "..//..//photogrammetrie//wrapper_for_c//photoST.h"


int main(void)
{
  //values
  double m_x_global,m_y_global,m_z_global;//,m_stdx_global,m_stdy_global,m_stdz_global;

  double d_begin = 3.0;
  double d_end   =30.0;
  double d_step  = 0.5;
  double d_i = d_begin;

  double m_begin,n_begin,m_end,n_end;

#if defined(_MSC_VER)
  char cam_left[]  = {"AVT_Stingray_right_left_cam1.inikalib.ini"};
  char cam_right[] = {"AVT_Stingray_right_right_cam2.inikalib.ini"};
#else
	char cam_left[]  = {"/home/steffen/projekte/Lehmann_und_Partner/photogrammetrie_dll/example_photogrammetrie/cam_calib/AVT_Stingray_right_left_cam1.inikalib.ini"};
	char cam_right[] = {"/home/steffen/projekte/Lehmann_und_Partner/photogrammetrie_dll/example_photogrammetrie/cam_calib/AVT_Stingray_right_right_cam2.inikalib.ini"};
#endif

//################################

#if defined(_MSC_VER)
  puts("access to the Photogrammetrie.dll\n");
#else
  puts("access to the Photogrammetrie.so\n");
#endif



//################################
  del_all();

  printf("\n##################################\n");
  printf("forwart intersection example 1:\n");

    //fill the camera list with one cam and on bpoint and than the next pair
    //create camera left with a bpoint
    addCam(1040,1392,0.00465,0.0,0.0,0.0,0.0,0.0,0.0,-8.3481198,0.00089,0.00472,-0.00284666,4.33195e-05,0,1.01299e-05,-2.01898e-05,-0.000134369,-4.26305e-05,2.427375,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
    addBPoint(272.0,325.0);

    //create camera right with a bpoint
    addCam(1040,1392,0.00465,-1114.242911,-2.313123752,-108.0708656,-0.006391017146,-0.1810263138,0.007287021954,-8.3789952,0.08349,0.00098,-0.00285721,4.69797e-05,0,8.79895e-05,6.01291e-05,3.13052e-05,-8.65479e-05,2.427375,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
    addBPoint(223.0,300.0);

    //calculate of the forward intersection
    calculate();

    //showVariablesAndInfos();

    printf("\nforwart intersection calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_local(),get_y_local(),get_z_local(),get_stdx_local(),get_stdy_local(),get_stdz_local());
    printf("\nforwart intersection control: (-1741.225369 , 799.567923 , -7397.498111)  std(2.136106,1.646223,12.459384)\n");

    //delete all values (depends of the internal List of bpoints)
    del_all();


//################################

  printf("\n##################################\n");
  printf("forwart intersection example 2:\n");

                 //info -- calculation from the calibration --
				 /* calibration
				 ###### forward intersection ########
				  picture left:  RechtsVorn_13.BMP  picture right: RechtsHinten_13.BMP
				  BP_left :126 (  756.000,  345.000)   BP_right :126 (  401.000,  370.000)
				  ###### end forward intersection ########
				  ######  boreside transformation ########
				  car pos left  : (  641754.64076, 5638452.77658,     296.79691)  rot (0.67666,      -0.16642,     147.47308)   have pic's the same car pos :1
				  UTM global    : (  641748.32241, 5638451.20961,     299.20626)  0.00000 0.00000 0.00000
				  UTM soll P_ref: (  641748.32088, 5638451.20780,     299.20560)
				  soll - ist    : (      -0.0-1062.3945612937441,1587.5888858836174,-7728.733215755935)0153,      -0.00180,      -0.00066)
				 ###### end boreside transformation ########
				 */


  addCam2(cam_left);
  addBPoint(756.000,345.000);

  addCam2(cam_right);
  addBPoint(401.000,370.000);

  addGlobalCarReferencePoint(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
  addGlobalCarReferencePoint_std( 1.0 , 1.0 , 2.0 , 0.1 , 0.1 , 0.2 );
  setGlobalReferenceFrame();//todo in process -> for EPS data

  //calculate of the forward intersection
  calculate();

  //showVariablesAndInfos();

  printf("\n");
  printf("\nforwart intersection calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_local(),get_y_local(),get_z_local(),get_stdx_local(),get_stdy_local(),get_stdz_local());
  printf("\nforwart intersection control: ((       0.40655,       0.50524,      -3.78090))\n");


  printf("\nbore side calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_global(),get_y_global(),get_z_global(),get_stdx_global(),get_stdy_global(),get_stdz_global());
  printf("\nbore side control: (641748.32241, 5638451.20961,     299.20626)");
  printf("\ntestfield control: (641748.32088, 5638451.20780,     299.20560)\n");

  //save the values for the next example
  m_x_global = get_x_global();//Easting
  m_y_global = get_y_global();//Northing
  m_z_global = get_z_global();//ell. Height


  //delete all values (depends of the internal List of bpoints)
  del_all();

//##################################

  printf("\n##################################\n");
  printf("forwart intersection example 2A:\n");

      //info -- calculation from the calibration --
      /* calibration
         ###### forward intersection ########
         picture left:  RechtsVorn_13.BMP  picture right: RechtsHinten_13.BMP
         BP_left :126 (  756.000,  345.000)   BP_right :126 (  401.000,  370.000)
         ###### end forward intersection ########
         ######  boreside transformation ########
         car pos left  : (  641754.64076, 5638452.77658,     296.79691)  rot (0.67666,      -0.16642,     147.47308)   have pic's the same car pos :1
         UTM global    : (  641748.32241, 5638451.20961,     299.20626)  0.00000 0.00000 0.00000
         UTM soll P_ref: (  641748.32088, 5638451.20780,     299.20560)
         soll - ist    : (      -0.0-1062.3945612937441,1587.5888858836174,-7728.733215755935)0153,      -0.00180,      -0.00066)
         ###### end boreside transformation ########
      */

  addCam( 960, 1280,
          0.00645,                            
          0, 0, 0,                              
          0, 0, 0,                              
          -5.96131, 0.09154, 0.05499,           
          -0.00392312, 5.3042e-05, 0,           
          3.41561e-05, 1.66584e-05,            
          0.000115336, 3.44961e-05,            
          3.1,
          1.810038117, 1.997471691, 2.954505537,
          0, 0, 0,                              
          1.236546986, -0.6468711578, -0.222276496,
          0, 0, 0);
  addBPoint(756.000,345.000);

  addCam( 960, 1280, 
          0.00645,                                   
          1.694736602, -0.5066248238, 1.913592406,     
          0.06547699024, -0.02425883623, 0.01597899627,
          -5.96062, -0.02699, 0.00748,                 
          -0.00392089, 5.0132e-05, 0,                  
          -2.96244e-05, -0.000116111,                 
          -8.63535e-05, -7.70085e-05,                 
          3.1,                                       
          1.886692106, -0.6072478387, 2.940466502,     
          0, 0, 0,                                     
          1.312015813, -0.6836290131, -0.1604868833,   
          0, 0, 0 );
  addBPoint(401.000,370.000);

  addGlobalCarReferencePoint(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
  setGlobalReferenceFrame();//todo in process -> for EPS data

  //calculate of the forward intersection
  calculate();

  //showVariablesAndInfos();

  printf("\n");
  printf("\nforwart intersection calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_local(),get_y_local(),get_z_local(),get_stdx_local(),get_stdy_local(),get_stdz_local());
  printf("\nforwart intersection control: ((       0.40655,       0.50524,      -3.78090))\n");


  printf("\nbore side calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_global(),get_y_global(),get_z_global(),get_stdx_global(),get_stdy_global(),get_stdz_global());
  printf("\nbore side control: (641748.32241, 5638451.20961,     299.20626)");
  printf("\ntestfield control: (641748.32088, 5638451.20780,     299.20560)\n");

  //save the values for the next example
  m_x_global = get_x_global();//Easting
  m_y_global = get_y_global();//Northing
  m_z_global = get_z_global();//ell. Height


  //delete all values (depends of the internal List of bpoints)
  del_all();




//##################################

  printf("\n#########################################\n");
  printf("calc back from a global point to left cam :\n");


  addCam2(cam_left);
  addGlobalMeasurementPoint(m_x_global,m_y_global,m_z_global);

  addGlobalCarReferencePoint(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
  setGlobalReferenceFrame();//todo in process -> for EPS data

  calculate();

  //showVariablesAndInfos();

  printf("\n");
  printf("\npicture point cam left calc   :  m: %lf , n: %lf ",get_m(),get_n());
  printf("\npicture point cam left control: ( 756.000 , 345.000 )\n");

  //delete all values (depends of the internal List of bpoints) is extreme important!!
  del_all();


//##################################

  printf("\n############################################\n");
  printf("calc back from a global point to right cam :\n");


  addCam2(cam_right);
  addGlobalMeasurementPoint(m_x_global,m_y_global,m_z_global);

  addGlobalCarReferencePoint(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
  setGlobalReferenceFrame();//todo in process -> for EPS data

  calculate();

  //showVariablesAndInfos();

  printf("\n");
  printf("\npicture point cam left calc   :  m: %lf , n: %lf ",get_m(),get_n());
  printf("\npicture point cam left control: ( 401.000,370.000 )\n");

  //delete all values (depends of the internal List of bpoints) is extreme important!!
  del_all();

//##################################

		printf("\n############################################\n");
		printf("Epipolargeomerie:\n");

		//double d_begin = 3.0;
		//double d_end   =30.0;
		//double d_step  = 0.5;
		//double d_i = d_begin;

		addCam2(cam_left);
		addBPoint(756.0,345.0);
		addGlobalCarReferencePoint_CamSetGlobal(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);

		addCam2(cam_right);
		addGlobalCarReferencePoint_CamSetGlobal(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);

		printf("\n");
		//double m_begin,n_begin,m_end,n_end;

		while( d_i <= d_end )
		{
		 setDistanceForEpipolarLine(d_i);
		 calculate();

		 if(d_i==d_begin)
		 {
		     m_begin=get_m();
		     n_begin=get_n();
		 }
		 if(d_i==d_end)
		 {
             m_end=get_m();
             n_end=get_n();
		 }

		 printf("\none point in %lf m distance on the epipolar line:   m: %lf , n: %lf ",d_i,get_m(),get_n());

		 d_i+=d_step;
		}



		printf("\ndistance of %lf m in pixel of the epipolar line:  dm: %lf , dn: %lf ",d_i,(m_begin-m_end),(n_begin-n_end));

		//delete all values (depends of the internal List of bpoints)
		del_all();


  //##################################

  printf("\n############################################\n");
  printf("Mono Photogrammetrie only for testing!!!:\n");

  //E ground car: ( n:  (      -0.00278,       0.02638,      -0.99965) , p:       0.20855) (test ground plane from UNO (TUD))

  addCam2(cam_left);
  addBPoint(756.000,345.000);
  addRefGroundSurface(0.0,0.0,1.0,2.44); //plane only for Point 126 picture 13!!

  addGlobalCarReferencePoint(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
  setGlobalReferenceFrame();//todo in process -> for EPS data

  //showVariablesAndInfos();

  //calculate of the forward intersection
  calculate();


  printf("\nmono measurenment calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_local(),get_y_local(),get_z_local(),get_stdx_local(),get_stdy_local(),get_stdz_local());
  printf("\nmono measurenment control: (not available -> this function only for testing)\n");
  printf("\nmono measurenment control: ( 0.410760 , 0.509896 , -3.820846 )\n");

  printf("\nbore side calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_global(),get_y_global(),get_z_global(),get_stdx_global(),get_stdy_global(),get_stdz_global());
  printf("\nbore side control: (not available -> this function only for testing)");
  printf("\nbore side control: (641748.283425 , 5638451.200895 , 299.200014)");

  //delete all values (depends of the internal List of bpoints)
  del_all();

//##################################

		printf("\n############################################\n");
		printf("multi car position photogrammetrie:\n");

		setGlobalReferenceFrame();//todo in process -> for EPS data

		//picture 13
		addCam2(cam_left);
		addBPoint(756.000,345.000);
		addGlobalCarReferencePoint_CamSetGlobal(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);

		//picture 13
		addCam2(cam_right);
		addBPoint(401.000,370.000);
		addGlobalCarReferencePoint_CamSetGlobal(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);

		//picture 12
		addCam2(cam_left);
		addBPoint( 715.0 , 325.0 );
		addGlobalCarReferencePoint_CamSetGlobal(641754.9447785,5638453.2901614,296.8227930,0.0109420,-0.9424530,212.0283640,50.8803453,11.0150821);

		//picture 12
		addCam2(cam_right);
		addBPoint( 395.0, 360.0 );
		addGlobalCarReferencePoint_CamSetGlobal(641754.9447785,5638453.2901614,296.8227930,0.0109420,-0.9424530,212.0283640,50.8803453,11.0150821);

		//picture 10
		addCam2(cam_left);
		addBPoint( 985.0 , 369.0);
		addGlobalCarReferencePoint_CamSetGlobal(641753.5016304,5638454.2034647,296.7935520,-0.1139750,-0.5450980,190.7414720,50.8803539,11.0150620);

		//picture 10
		addCam2(cam_right);
		addBPoint( 506.0 , 381.0 );
		addGlobalCarReferencePoint_CamSetGlobal(641753.5016304,5638454.2034647,296.7935520,-0.1139750,-0.5450980,190.7414720,50.8803539,11.0150620);

		//picture 1
		addCam2(cam_left);
		addBPoint( 65.0 , 305.0);
		//1001;50.8803713;11.0150901;296.8429180;-0.2612280;-1.5055340;219.9422050;15:47:18;1;641755.4308509;5638456.1936776
		addGlobalCarReferencePoint_CamSetGlobal(641755.4308509,5638456.1936776,296.8429180,-0.2612280,-1.5055340,219.9422050,50.8803713,11.0150901);


		//calculate of the forward intersection
		calculate();

		//showVariablesAndInfos();

		printf("\n");
		printf("\nforwart intersection calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_local(),get_y_local(),get_z_local(),get_stdx_local(),get_stdy_local(),get_stdz_local());
		printf("\nforwart intersection control: (641748.309525 , 5638451.210544 , 299.207956  std(0.023276,0.012765,0.006509))\n");


		printf("\nbore side calc   :  x: %lf ,y: %lf z: %lf  std(%lf,%lf,%lf)",get_x_global(),get_y_global(),get_z_global(),get_stdx_global(),get_stdy_global(),get_stdz_global());
		printf("\nbore side control: (641748.309525 , 5638451.210544 , 299.207956)");
		printf("\ntestfield control: (641748.32088  , 5638451.20780  , 299.20560)\n");


		//delete all values (depends of the internal List of bpoints)
		del_all();



//##################################

        printf("\n############################################\n");
        printf("foot print:\n");

        setGlobalReferenceFrame();//todo in process -> for EPS data

        //picture 13
        addCam2(cam_left);
        addGlobalCarReferencePoint_CamSetGlobal(641754.64076,5638452.77658,296.79691,-0.2235410,-0.6600010,214.0967730,50.8803408,11.0150776);
        addRefGroundSurface(0.0,0.0,1.0,0.30); //test plane
        setDistanceForEpipolarLine(20.0); //max 20m in front of the picture

        //calculate of the forward intersection
        calculate();

        printf("\n Point left  up [x,y,z] : [%lf,%lf,%lf] ",get_FP_Easting(0),get_FP_Northing(0),get_FP_eHeight(0));
        printf("\n Point right up [x,y,z] : [%lf,%lf,%lf] ",get_FP_Easting(1),get_FP_Northing(1),get_FP_eHeight(1));
        printf("\n Point left  dw [x,y,z] : [%lf,%lf,%lf] ",get_FP_Easting(2),get_FP_Northing(2),get_FP_eHeight(2));
        printf("\n Point right dw [x,y,z] : [%lf,%lf,%lf] ",get_FP_Easting(3),get_FP_Northing(3),get_FP_eHeight(3));


        //delete all values (depends of the internal List of bpoints)
        del_all();

printf("\n\nend\n");

#if defined(_MSC_VER)
  getchar();
#endif

  return EXIT_SUCCESS;
}

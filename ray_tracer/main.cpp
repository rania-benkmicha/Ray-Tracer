#define SDL_MAIN_HANDLED

#include <math.h>
#include <vector>
#include <SDL.h>
#include <string>
#include <iostream>
#include "vecteur.h"
#include "sphere.h"
#include <fstream>
#include <sstream>
#include <omp.h>

#include <chrono>
//#include<thread>

struct Colors
{
	Vec3 white = { 255, 255, 255 };
	Vec3 yellow = { 255, 255, 0 };
	Vec3 black = { 0, 0, 0 };
	Vec3 red = { 255, 0, 110 };
	Vec3 green = { 110, 255,0 };
	Vec3 blue = { 0, 110, 255 };
	Vec3 li={246,220,161};
};

void Colorlimit(Vec3& clr)
{
	clr.x = (clr.x > 255) ? 255 : (clr.x < 0) ? 0 : clr.x;
	clr.y = (clr.y > 255) ? 255 : (clr.y < 0) ? 0 : clr.y;
	clr.z = (clr.z > 255) ? 255 : (clr.z < 0) ? 0 : clr.z;
}

void Ref(const std::vector<Sphere>& objects,const Vec3& pointOfInt,const Vec3& normal,Vec3& pixclr)
{
	Colors color;
	Vec3 pixclradd;
	

	for (auto& sphereelement : objects)
	{
		const Ray rayref(pointOfInt, normal * 30);//pour la deviation de rayon refléchi
		double t2;
		if (sphereelement.Intersects(rayref, t2))//reflexion successive
		{

			const Vec3 pointOfInt1 = rayref.getorigin() + rayref.getdirection()* t2;

			const Vec3 len1 = pointOfInt - pointOfInt1;

			const Vec3 normal1 = sphereelement.GetNormal(pointOfInt1);

			const double dotP1 = dot(len1.Normalize(), normal1.Normalize());

			
			pixclradd = sphereelement.clr * dotP1 * sphereelement.eclat;//eclat c'est la nature de surface
			//la camera regarde current sphere
			//coeffcient de lambert  pour la diffusion des couleurs 
			//on utilise spherb->clr pour qu'on aura la couleur des voisins sur current sphere


			if (sphereelement.name == "world")
				pixclradd = pixclradd - color.white/1.5;



			Colorlimit(pixclradd);
			pixclr = pixclradd + pixclr;
		}
		
	}
}

void ombre(const std::vector<Sphere>& objects,const Vec3& pointOfIntersection,const Vec3& normal,Vec3& pixclr,const Sphere& light)
{
	Colors color;
	
	double shadowIntensity = 2;//indice de refraction

	for (auto& sphereelement : objects)
	{
		const Ray ray2(pointOfIntersection, light.GetNormal(sphereelement.center) * -1);//le principe est d'atténuer la lumière si le rayon intersecte des objets en allant vers
		// la lumière (atténuation complète si un objet traversé est opaque
		//on parle de shadow ray technique 
		double t2;
		if (sphereelement.Intersects(ray2, t2))
		{
			const Vec3 pointOfIntersection2 = ray2.getorigin() + ray2.getdirection() * t2;
			const Vec3 len2 = pointOfIntersection - pointOfIntersection2;
			const Vec3 normal2 = sphereelement.GetNormal(pointOfIntersection2);
			const double dotP2 = dot(len2.Normalize(), normal2.Normalize());

			double shadow =  4 * shadowIntensity;
			Vec3 pixclrBounce = pixclr * dotP2 *shadow;//par analogie
			Colorlimit(pixclrBounce);

			pixclr = pixclr - pixclrBounce;
			
		}
	}
}

Vec3 Rendu(const Ray& ray, const Sphere& light, const std::vector<Sphere>& objects)
{
std::chrono::time_point < std::chrono::system_clock > start, end;

    start = std::chrono::system_clock::now();

	Colors color;
	Vec3 pixclr = color.black;

//#pragma omp  parallel for schedule(dynamic)

	for (auto& sphere : objects)
	{
		double t;
		double lightIntensity = 0.62;

		if (sphere.Intersects(ray, t)&& sphere.name=="light")
		{       pixclr=light.clr;}

		// il s'agit d'une intersection donc on voit l'objet d'ou il existe un rayon issu de la source lumineuse qui se dirige vers l'objet
		else if (sphere.Intersects(ray, t)&& sphere.name!="light")
		{
			const Vec3 pointOfIntersection = ray.getorigin()+ ray.getdirection() * t;
			const Vec3 len = light.center - pointOfIntersection;
			const Vec3 normal = sphere.GetNormal(pointOfIntersection);
			const double dotProduct = dot(len.Normalize(), normal.Normalize());//c'est le rpofuit scalaire entre la normale et la direction de lumiere

			pixclr =  sphere.clr*lightIntensity+(light.clr * dotProduct) * lightIntensity;
			//en lumiere  ambiante +lumiere diffusée
			//light.clr car on voulait la couleur de la source lumineuse sur l'objet//superpositioon selon phong
			
			

			if (sphere.name != "world" && sphere.name!="light")
				{Ref(objects, pointOfIntersection, normal, pixclr);}
				//on fait ici le phenomene de la reflexion successive entre les objets de la scene
				if (sphere.name!="light")
			ombre(objects, pointOfIntersection, normal, pixclr, light);
			
			Colorlimit(pixclr);
			
	}
	
	}
   end = std::chrono::system_clock::now();
        std::chrono::duration < double >elapsed_seconds = end - start;
    //std::cout<<elapsed_seconds.count()<<std::endl;

	
	return pixclr;
}


struct SmallScreen
{
	Colors color;
	int x, y;
	Sphere light;
	std::vector<Sphere>& objList;
	SDL_Renderer* renderer;
};


Vec3 RenderPixel(const SmallScreen& sd)
{
	Vec3 pixclr = sd.color.black;


	const Ray ray(Vec3(sd.x, sd.y, -1), Vec3(0, 0, 1));//l'axe de z est pointe vers l'interieur de la fenetre
	pixclr = Rendu(ray, sd.light, sd.objList);

	return pixclr;
}

void HandleEvents(
	bool& isRunning,
	Sphere& light,
	Sphere& world)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT)
		isRunning = false;

	if (event.type == SDL_MOUSEMOTION)
	{
		light.center.x = event.motion.x;
		light.center.y = event.motion.y;
	}

	/*if (event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0 && world.center.z > 580)
		{
			world.center.z -= 5;
			world.center.y -= 5;

			light.center.z -= 30;
		}

		if (event.wheel.y < 0 && world.center.z < 660)
		{
			world.center.z += 5;
			world.center.y += 5;

			light.center.z += 30;
		}

		event.wheel.y = 0;
	}*/
}

int main()
{
	const int W = 400;
	const int H = 400;
	//choix du fichier

std::cout<<"choisissez le fichier numero 1 ou 2 pour faire l'animation"<<std::endl;
char  num[2];
std::cin.get(num,2);//elle capte entree
//std::cout<<num<<std::endl;
//lire du fichier corrspondant 

std::vector <std::string> words;

std::string text=(std::string) num +".txt";
//std::cout<<text<<std::endl;

    std::ifstream data(text);//pour le test
    if (!data)
    { 
        std::cout << "could not open file" << std::endl;
        return 1;
    }    
    int nbligne=0;
    std::string str;
    while (std::getline(data, str))
    {
		nbligne++;
        std::istringstream ss(str);
        std::string value;
        while (getline(ss, value, '/'))
        {
            //std::cout << value << std::endl;
        words.push_back(value);}

    }
	//for(int j=0;j<words.size();j++){
		//std::cout<<words[j]<<std::endl;
	//}
   //std::cout<<nbligne<<std::endl;//test
	std::cin.ignore();
std::cout<<"Entrez svp le numéro 1 ou 2 pour choisir entre une animation que vous pouvez controler et un résulat que vous pouvez enregistrer"<<std::endl;
char  methode[3];
std::cin.get(methode,3);


 

	std::cout<<methode<<std::endl;
    //std::cin.get();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* win = 
		SDL_CreateWindow("Ray Tracer",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	

	//Sphere sphere(Vec3(W * 0.5, H * 0.5, 40), 30, color.white);
	//Sphere sphere1(Vec3(W * 0.3, H * 0.5, 20), 15, color.red);
	//Sphere sphere2(Vec3(W * 0.25, H * 0.35, 30), 20, color.green);
	//Sphere sphere3(Vec3(W * 0.85, H * 0.65, 40), 35, color.blue);


Colors color;
  
    //Sphere sphere3(Vec3(W*x,H*y, z), rayonn, cl);

	Vec3 worldColor = Vec3(94,0,182);
	Sphere world(Vec3(W * 0.5, H * 3.8, 650), 1400, worldColor,2.5,"world");

	Sphere light(Vec3(W * 0.4, H * 0.2, 0), 20, color.li,0,"light");

	

   
   
    std::vector<Sphere> objList;
   objList.push_back(world);
  //objList.push_back(light);
    for(int i=0;i<nbligne;i++){
		
            double x,y,z,rayonn,eclat;
  
            std::istringstream(words[i*6]) >> x;
            std::istringstream(words[i*6+1]) >> y;
            std::istringstream(words[i*6+2]) >> z;
            std::istringstream(words[i*6+3]) >> rayonn;
            std::istringstream(words[i*6+5]) >> eclat;

        Vec3 cl;
    
        if (words[i*6+4]=="color.white"){
            cl=color.white;
		}

        else if (words[i*6+4] =="color.blue"){
           cl=color.blue;
		}

        else if (words[i*6+4] =="color.red"){
            cl=color.red;
            }
        else if (words[i*6+4]== "color.green"){
        
            cl=color.green;
		}
		else if (words[i*6+4]== "color.black"){
		    cl=color.black;
		}
		else if (words[i*6+4] =="color.yellow"){
		    cl=color.yellow;
		} 


        //Sphere sphere3(Vec3(W*x,H*y, z), rayonn, cl);
        
        objList.push_back( Sphere(Vec3(W*x,H*y, z), rayonn, cl,eclat));
        //std::cout<<objList.size()<<"ici c paris"<<std::endl;
		
    }
for (auto& sphereB : objList){

	std::cout<<sphereB.radius<<std::endl;

}




if (methode[0]=='1'){

	bool isRunning = true;

	/**///interaction avecl'utilisateur
	while (isRunning)
	{
		HandleEvents(isRunning, light, world);//on controle la position de la lumiere avec souris




		for (int y = 0; y < H; ++y)
			for (int x = 0; x < W; ++x)
			{	
			    SmallScreen scrnData = { color, x, y, light, objList, renderer };
				Vec3 pixclr = RenderPixel(scrnData);
				Colorlimit(pixclr);

				SDL_SetRenderDrawColor(renderer, (int)pixclr.x, (int)pixclr.y, (int)pixclr.z, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}




     SDL_RenderPresent(renderer);
	 }
//enregistrer l'animation sous forme d'une image BMP ,ici la derniere instant del'animation va être enregistrer
SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, 400,400, 32, 0, 0, 0, 0);

   if(pScreenShot)
   {
      // Read the pixels from the current render target and save them onto the surface
      SDL_RenderReadPixels(renderer ,NULL, SDL_GetWindowPixelFormat(win), pScreenShot->pixels, pScreenShot->pitch);

      // Create the bmp screenshot file
      SDL_SaveBMP(pScreenShot, "Screenshot.bmp");

      // Destroy the screenshot surface
      SDL_FreeSurface(pScreenShot);
   }

}

else if(methode[0]=='2'){

for (int y = 0; y < H; ++y)
			for (int x = 0; x < W; ++x)
			{	
			    SmallScreen scrnData = { color, x, y, light, objList, renderer };
				Vec3 pixclr = RenderPixel(scrnData);
				Colorlimit(pixclr);

				SDL_SetRenderDrawColor(renderer, (int)pixclr.x, (int)pixclr.y, (int)pixclr.z, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
			 SDL_RenderPresent(renderer);
	 
//enregistrer l'animation sous forme d'une image BMP
SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, 400,400, 32, 0, 0, 0, 0);

   if(pScreenShot)
   {
      // Read the pixels from the current render target and save them onto the surface
      SDL_RenderReadPixels(renderer ,NULL, SDL_GetWindowPixelFormat(win), pScreenShot->pixels, pScreenShot->pitch);

      // Create the bmp screenshot file
      SDL_SaveBMP(pScreenShot, "Screenshot.bmp");

      // Destroy the screenshot surface
      SDL_FreeSurface(pScreenShot);
   }

		

    SDL_Delay(10000);//on ll'utilise si on ne veut pas controler la lumiere avec souris pour ne pas tomber dans une boule infinie
}
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(win);
	
	SDL_Quit();

	return 0;
}

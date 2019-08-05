
#include "Loader.h"
#include "Global.h"


void createVertexBuffer(int nrOfVertices, TriangleVertex ArrOfVert[]) {
	gnrOfVert[nrOfVertexBuffers] = nrOfVertices;

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * nrOfVertices;

	//int size = sizeof(gMap);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)ArrOfVert;

	// create a Vertex Buffer
	HRESULT error;
	error = gDevice->CreateBuffer(&bufferDesc, &data, &ppVertexBuffers[nrOfVertexBuffers]);
	nrOfVertexBuffers++;
}

void loadHeightMap(char* filename) //24 bit colour depth
{
	FILE *fileptr; //filepointer
	BITMAPFILEHEADER bitmapFileHeader; //struct containing file information
	BITMAPINFOHEADER bitmapInfoHeader; //struct contatining image information
	int imageSize, index;
	unsigned char height;
	//Heightmap heightmap;

	//Open the file
	fileptr = fopen(filename, "rb");
	if (fileptr == 0)
	{
		return;
	}

	//Read headers
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fileptr);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fileptr);

	//retrieve width and height of object
	g_heightmap.imageWidth = bitmapInfoHeader.biWidth;
	g_heightmap.imageHeight = bitmapInfoHeader.biHeight;

	//get size of image in bytes
	int padding = g_heightmap.imageWidth * 3 % 4; //Ta det sen
	if (padding > 0)
	{
		padding = 4 - padding;
	}

	imageSize = (g_heightmap.imageHeight * g_heightmap.imageWidth * 3) + (g_heightmap.imageHeight * padding); //3 is for the three values RGB, added 2 byte per row for bumper data.

	//array of image data
	unsigned char* bitmapImage = new unsigned char[imageSize];

	//Find start of image data
	fseek(fileptr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//read data into bitmapimage
	fread(bitmapImage, 1, imageSize, fileptr);

	//close file
	fclose(fileptr);

	//array of vertice positions
	g_heightmap.verticesPos = new XMFLOAT3[g_heightmap.imageHeight * g_heightmap.imageWidth];

	int counter = 0; //Eftersom bilden är i gråskala så är alla värden RGB samma värde, därför läser vi bara R

	int heightfactor = int(25.50f * 0.3f); //mountain smoothing

	//read and put vertex position
	for (int i = 0; i < g_heightmap.imageHeight; i++)
	{
		for (int j = 0; j < g_heightmap.imageWidth; j++)
		{
			height = bitmapImage[counter];
			index = (g_heightmap.imageWidth * i) + j;

			g_heightmap.verticesPos[index].x = (float)j;
			g_heightmap.verticesPos[index].y = (float)height / heightfactor;
			g_heightmap.verticesPos[index].z = (float)i;
			counter += 3; //Skipping GB
		}
		counter += padding; //Skip padding info at the end of each row.
	}

	nrOfHMVert = 4 + ((g_heightmap.imageWidth - 2) * 4 * 2) + ((g_heightmap.imageHeight - 2) * 4 * 2) + ((g_heightmap.imageHeight - 1) * (g_heightmap.imageWidth - 1) * 6);

	//TriangleVertex* map = new TriangleVertex[gnrOfVert[nrOfVertexBuffers]];
	g_map = new TriangleVertex[nrOfHMVert];

	int vertNr = 0;

	for (int i = 0; i < g_heightmap.imageHeight - 1; i++)
	{
		int X = (i * g_heightmap.imageWidth);
		int Y = ((i + 1) * g_heightmap.imageWidth);

		for (int k = 0; k < g_heightmap.imageWidth - 1; k++)
		{
			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[Y].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[Y].y;
			g_map[vertNr].z = g_heightmap.verticesPos[Y].z - (g_heightmap.imageHeight / 2);
			/*Colour*/

			/*UV*/
			g_map[vertNr].u = 0.0f;
			g_map[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[X + 1].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[X + 1].y;
			g_map[vertNr].z = g_heightmap.verticesPos[X + 1].z - (g_heightmap.imageHeight / 2);
			/*Colour*/

			/*UV*/
			g_map[vertNr].u = 1.0f;
			g_map[vertNr].v = 0.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[X].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[X].y;
			g_map[vertNr].z = g_heightmap.verticesPos[X].z - (g_heightmap.imageHeight / 2);
			/*Colour*/

			/*UV*/
			g_map[vertNr].u = 0.0f;
			g_map[vertNr].v = 0.0f;


			vertNr++;
			X++;

			/*-------------------------------*/
			/*Next triangle*/

			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[Y].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[Y].y;
			g_map[vertNr].z = g_heightmap.verticesPos[Y].z - (g_heightmap.imageHeight / 2);

			/*UV*/
			g_map[vertNr].u = 0.0f;
			g_map[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[X + g_heightmap.imageWidth].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[X + g_heightmap.imageWidth].y;
			g_map[vertNr].z = g_heightmap.verticesPos[X + g_heightmap.imageWidth].z - (g_heightmap.imageHeight / 2);

			/*UV*/
			g_map[vertNr].u = 1.0f;
			g_map[vertNr].v = 1.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			g_map[vertNr].x = g_heightmap.verticesPos[X].x - (g_heightmap.imageWidth / 2);
			g_map[vertNr].y = g_heightmap.verticesPos[X].y;
			g_map[vertNr].z = g_heightmap.verticesPos[X].z - (g_heightmap.imageHeight / 2);

			/*UV*/
			g_map[vertNr].u = 1.0f;
			g_map[vertNr].v = 0.0f;


			vertNr++;
			Y++;
		}
	}

	HRESULT hr0 = CreateDDSTextureFromFile(gDevice, L"Objects/Materials/Lava16x16.dds", &gMapTextureResource[0], &gMapTexturesSRV[0]);
	HRESULT hr1 = CreateDDSTextureFromFile(gDevice, L"Objects/Materials/Sand16x16.dds", &gMapTextureResource[1], &gMapTexturesSRV[1]);
	HRESULT hr2 = CreateDDSTextureFromFile(gDevice, L"Objects/Materials/Grass16x16.dds", &gMapTextureResource[2], &gMapTexturesSRV[2]);
	HRESULT hr3 = CreateDDSTextureFromFile(gDevice, L"Objects/Materials/Stone16x16.dds", &gMapTextureResource[3], &gMapTexturesSRV[3]);

	delete[] bitmapImage;
}

void loadMultiTextureMap(char* filename)
{
	FILE *fileptr; //filepointer
	BITMAPFILEHEADER bitmapFileHeader; //struct containing file information
	BITMAPINFOHEADER bitmapInfoHeader; //struct contatining image information
	int imageSize, index;
	XMINT2 heightmap;

	//Open the file
	fileptr = fopen(filename, "rb");
	if (fileptr == 0)
	{
		return;
	}

	//Read headers
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fileptr);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fileptr);

	//retrieve width and height of object
	heightmap.x = bitmapInfoHeader.biWidth;
	heightmap.y = bitmapInfoHeader.biHeight;

	//get size of image in bytes
	int padding = heightmap.x * 3 % 4; //The BMP file has padding on the end of each row. This is a short function for calculating it.
	if (padding > 0)
	{
		padding = 4 - padding;
	}

	imageSize = (heightmap.y * heightmap.x * 3) + (heightmap.y * padding); //3 is for the three values RGB, added 2 byte per row for bumper data.

	//array of image data
	unsigned char* bitmapImage = new unsigned char[imageSize];

	//Find start of image data
	fseek(fileptr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//Read data into bitmapimage
	fread(bitmapImage, 1, imageSize, fileptr);

	//close file
	fclose(fileptr);

	int counter = 0; //Since the picture is in black and white we only have to read the red-value, as the green and blue value would be the same.

	XMFLOAT3* RGB = new XMFLOAT3[heightmap.x * heightmap.y];

	//read and put vertex position
	for (int i = 0; i < heightmap.y; i++)
	{
		for (int j = 0; j < heightmap.x; j++)
		{
			index = (g_heightmap.imageWidth * i) + j;

			float r = 0, g = 0, b = 0;

			b = bitmapImage[counter];
			counter++;
			g = bitmapImage[counter];
			counter++;
			r = bitmapImage[counter];
			counter++;

			float total = r + g + b;
			if (total < 1) total = 1;

			RGB[index].x = r / total;
			RGB[index].y = g / total;
			RGB[index].z = b / total;
		}
		counter += padding; //Skip padding info at the end of each row.
	}

	int vertNr = 0;

	for (int i = 0; i < g_heightmap.imageHeight - 1; i++)
	{
		int X = (i * g_heightmap.imageWidth);
		int Y = ((i + 1) * g_heightmap.imageWidth);

		for (int k = 0; k < g_heightmap.imageWidth - 1; k++)
		{
			/*Position*/
			g_map[vertNr].r = RGB[Y].x;
			g_map[vertNr].g = RGB[Y].y;
			g_map[vertNr].b = RGB[Y].z;

			vertNr++;

			/*..............................*/

			/*Position*/
			g_map[vertNr].r = RGB[X + 1].x;
			g_map[vertNr].g = RGB[X + 1].y;
			g_map[vertNr].b = RGB[X + 1].z;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			g_map[vertNr].r = RGB[X].x;
			g_map[vertNr].g = RGB[X].y;
			g_map[vertNr].b = RGB[X].z;
			/*Colour*/

			vertNr++;
			X++;

			/*-------------------------------*/
			/*Next triangle*/

			/*Position*/
			g_map[vertNr].r = RGB[Y].x;
			g_map[vertNr].g = RGB[Y].y;
			g_map[vertNr].b = RGB[Y].z;

			vertNr++;

			/*..............................*/

			/*Position*/
			g_map[vertNr].r = RGB[X + g_heightmap.imageWidth].x;
			g_map[vertNr].g = RGB[X + g_heightmap.imageWidth].y;
			g_map[vertNr].b = RGB[X + g_heightmap.imageWidth].z;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			g_map[vertNr].r = RGB[X].x;
			g_map[vertNr].g = RGB[X].y;
			g_map[vertNr].b = RGB[X].z;

			vertNr++;
			Y++;
		}
	}

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * nrOfHMVert;

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)g_map;

	// create a Vertex Buffer
	HRESULT error;
	error = gDevice->CreateBuffer(&bufferDesc, &data, &heightmapBuffer);
	delete[] g_map;
	delete[] bitmapImage;
	delete[] RGB;
}

void LoadObjectFile(char* filename, XMINT3 offset)
{
	TriangleVertex* object = nullptr;
	FILE* fileptr;
	fileptr = fopen(filename, "r");
	if (fileptr == NULL)
	{
		return;
	}
	int loopControl = 1;
	char line[128];

	XMFLOAT3* arrOfVertices = new XMFLOAT3[100]; //positionVertices
	int arrSize = 100;
	int nrOfVert = 0;

	bool textureCordStart = false;
	XMFLOAT2* arrOfTxtCord = nullptr; //TextureCoordinates
	int nrOfTxtCord = 0;
	int txtCoordArrSize = 0;

	bool normalStart = false;
	XMFLOAT3* arrOfNormals = nullptr; //NormalCoordinates
	int nrOfNormals = 0;
	int normArrSize = 0;

	char materialPath[50] = "Objects/OBJs/";

	int nrOfFaces = 0;

	bool indexStart = false;
	XMINT3* arrOfIndex = nullptr;
	int objArrSize = 0;

	while (loopControl != EOF)
	{
		loopControl = fscanf(fileptr, "%s", line);
		if (loopControl == EOF) {}
		else
		{
			if (strcmp(line, "v") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				if (nrOfVert == arrSize)
				{
					XMFLOAT3* tempArr = new XMFLOAT3[arrSize + 50];
					for (int i = 0; i < arrSize; i++)
					{
						tempArr[i] = arrOfVertices[i];
					}
					delete arrOfVertices;
					arrOfVertices = tempArr;

					arrSize += 50;
				}
				arrOfVertices[nrOfVert] = vertex;
				nrOfVert++;
			}
			else if (strcmp(line, "vt") == 0)
			{
				if (textureCordStart == false)
				{
					txtCoordArrSize = nrOfVert;
					arrOfTxtCord = new XMFLOAT2[txtCoordArrSize];
					textureCordStart = true;
				}
				if (nrOfTxtCord == txtCoordArrSize)
				{
					XMFLOAT2* tempArr = new XMFLOAT2[txtCoordArrSize + 50];
					for (int i = 0; i < txtCoordArrSize; i++)
					{
						tempArr[i] = arrOfTxtCord[i];
					}
					delete arrOfTxtCord;
					arrOfTxtCord = tempArr;

					txtCoordArrSize += 50;
				}
				XMFLOAT2 vertex;
				fscanf(fileptr, "%f %f\n", &vertex.x, &vertex.y);

				arrOfTxtCord[nrOfTxtCord] = vertex;
				arrOfTxtCord[nrOfTxtCord].y = 1 - arrOfTxtCord[nrOfTxtCord].y;
				nrOfTxtCord++;
			}
			else if (strcmp(line, "vn") == 0)
			{
				if (normalStart == false)
				{
					normArrSize = nrOfVert;
					arrOfNormals = new XMFLOAT3[normArrSize];
					normalStart = true;
				}
				if (nrOfNormals == normArrSize)
				{
					XMFLOAT3* tempArr = new XMFLOAT3[normArrSize + 50];
					for (int i = 0; i < normArrSize; i++)
					{
						tempArr[i] = arrOfNormals[i];
					}
					delete arrOfNormals;
					arrOfNormals = tempArr;

					normArrSize += 50;
				}

				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				arrOfNormals[nrOfNormals] = vertex;
				nrOfNormals++;
			}
			else if (strcmp(line, "mtllib") == 0)
			{
				char materialName[50];
				fscanf(fileptr, "%s", materialName);
				std::strcat(materialPath, materialName);

				loadTexture(materialPath);

			}
			else if (strcmp(line, "f") == 0)
			{
				XMINT3 vertex[3];
				fscanf(fileptr, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex[0].x, &vertex[0].y, &vertex[0].z, &vertex[1].x, &vertex[1].y, &vertex[1].z, &vertex[2].x, &vertex[2].y, &vertex[2].z);
				/*x innehåller vertex positioner, y innehåller texture, z innehåller normaler*/
				if (indexStart == false)
				{
					objArrSize = int(nrOfVert * 1.5);
					object = new TriangleVertex[objArrSize];
					indexStart = true;
				}
				if (nrOfFaces + 3 >= objArrSize)
				{
					TriangleVertex* tempArr = new TriangleVertex[objArrSize + 50];
					for (int i = 0; i < nrOfFaces; i++)
					{
						tempArr[i] = object[i];
					}
					delete object;
					object = tempArr;

					objArrSize += 50;
				}
				for (int i = 0; i < 3; i++)
				{
					object[nrOfFaces + i].x = arrOfVertices[vertex[i].x - 1].x + offset.x;
					object[nrOfFaces + i].y = arrOfVertices[vertex[i].x - 1].y + offset.y;
					object[nrOfFaces + i].z = arrOfVertices[vertex[i].x - 1].z + offset.z;

					object[nrOfFaces + i].u = arrOfTxtCord[vertex[i].y - 1].x;
					object[nrOfFaces + i].v = arrOfTxtCord[vertex[i].y - 1].y;

					object[nrOfFaces + i].r = 0.5f;
					object[nrOfFaces + i].g = 0.0f;
					object[nrOfFaces + i].b = 0.5f;
				}
				nrOfFaces += 3;
			}
		}
	}
	fclose(fileptr);

	TriangleVertex* objectArray = new TriangleVertex[nrOfFaces];
	for (int i = 0; i < nrOfFaces; i++)
	{
		objectArray[i] = object[i];
	}

	createVertexBuffer(nrOfFaces, objectArray);

	delete[] objectArray;
	delete[] arrOfVertices;
	delete[] arrOfTxtCord;
	delete[] arrOfNormals;
	delete[] arrOfIndex;
	delete object;
}

void loadTexture(const char* filepath)
{
	FILE* fileptr;
	fileptr = fopen(filepath, "r");
	if (fileptr == NULL)
	{
		return;
	}

	int loopControl = 1;
	char line[128];

	while (loopControl != EOF)
	{
		loopControl = fscanf(fileptr, "%s", line);
		if (loopControl == EOF) {}
		else
		{
			if (strcmp(line, "Ka") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				gReflection[nrOfVertexBuffers].a_r = vertex.x;
				gReflection[nrOfVertexBuffers].a_g = vertex.y;
				gReflection[nrOfVertexBuffers].a_b = vertex.z;

			}
			else if (strcmp(line, "Kd") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				gReflection[nrOfVertexBuffers].d_r = vertex.x;
				gReflection[nrOfVertexBuffers].d_g = vertex.y;
				gReflection[nrOfVertexBuffers].d_b = vertex.z;

			}

			if (strcmp(line, "Ks") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				gReflection[nrOfVertexBuffers].s_r = vertex.x;
				gReflection[nrOfVertexBuffers].s_g = vertex.y;
				gReflection[nrOfVertexBuffers].s_b = vertex.z;
			}
			if (strcmp(line, "Ns") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f\n", &vertex.x);
				gReflection[nrOfVertexBuffers].s_p = vertex.x;
			}
			else if (strcmp(line, "map_Kd") == 0)
			{
				char *pTexturePath = new char[50];
				fscanf(fileptr, "%s", pTexturePath);

				wchar_t wideChar[50];

				std::mbstowcs(wideChar, pTexturePath, 50);

				delete[] pTexturePath;

				CreateDDSTextureFromFile(gDevice, wideChar, &gTexture2D[nrOfVertexBuffers], &gTextureSRV[nrOfVertexBuffers]);
			}
		}

	}
	fclose(fileptr);
}
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <zlib.h>      
#include <unzip.h>
#include <3ds.h>

//code from 2025/12, i tried making a 3DS program to automatically insall everything needed

int main()
{

	gfxInitDefault(); //init screen services

    const char *files[] = {
        "https://github.com/AtexBg/3ds-fbge/releases/download/v0.1.0/linux_3ds-fbge_install.zip", //linux install package
        "https://github.com/AtexBg/3ds-fbge/releases/latest/download/fbge", //DE binary
    };

    const char *paths[] = { //paths to save the files
        "sdmc:/linux_3ds-fbge_install.zip",
        "sdmc:/fbge",
    };

    for (int i = 0; i < 2; i++) { //download both files
        FILE *linuxArchiveOutputPath = fopen(paths[i], "wb");

        curl_easy_setopt(curl, CURLOPT_URL, files[i]);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, linuxArchiveOutputPath);
        curl_easy_perform(curl);

        fclose(linuxArchiveOutputPath);
    }

    //
    unzFile zip = unzOpen("sdmc:/linux_3ds-fbge_install.zip");
    unzGoToFirstFile(zip);
    //thanks to some random dude on the internet for the unzip code i copy-pasted from his tutorial
    while (unzGoToNextFile(zip) == UNZ_OK){
        char filename[256];
        unz_file_info info;

        unzGetCurrentFileInfo(zip, &info, filename, sizeof(filename), NULL, 0, NULL, 0);

        unzOpenCurrentFile(zip);

        char outPath[300];
        sprintf(outPath, "sdmc:/%s", filename);

        FILE *linuxArchiveOutputPath = fopen(outPath, "wb");
        if (!linuxArchiveOutputPath) continue;

        char buf[4096];
        int read;
        while ((read = unzReadCurrentFile(zip, buf, sizeof(buf))) > 0) {
            fwrite(buf, 1, read, linuxArchiveOutputPath);
        }

        fclose(linuxArchiveOutputPath);
        unzCloseCurrentFile(zip);

    }

    while(aptMainLoop()){
		//exit when user press Y
		hidScanInput();
		if(keysHeld()&KEY_Y){break;}

		//some things for graphics later
		gfxSwapBuffersGpu();
		gspWaitForEvent(GSPGPU_EVENT_VBlank0, false);
	}

	//closing services
	gfxExit();
	return 0;
}

/* BaseSequenceIDs.cpp :
 * Author: Maxie D. Schmidt (maxieds@gmail.com)
 * Created: 2019.10.30
 */

#include <openssl/sha.h>

#include <iostream>
#include <iomanip>
#include <sstream>

#include "BaseSequenceIDs.h"
#include "TerminalPrinting.h"

/* Hash Scheme: [FIRST 8 BYTES OF BSEQ + LAST 8 BYTES OF BSEQ + 256/8=32 BYTES OF SHA256 HASH] */
std::string HashBaseSequence(const char *baseSeq) {
     
     if(baseSeq == NULL) {
          return "";
     }

     int baseSeqLen = strlen(baseSeq);
     unsigned char hashBuf[SHA256_DIGEST_LENGTH];
     SHA256_CTX sha256HasherObj;
     SHA256_Init(&sha256HasherObj);
     SHA256_Update(&sha256HasherObj, baseSeq, baseSeqLen);
     SHA256_Final(hashBuf, &sha256HasherObj);

     std::stringstream ss;
     for(int hidx = 0; hidx < SHA256_DIGEST_LENGTH; hidx++) {
          ss << std::hex << std::setw(2) << std::setfill('0') << (int) hashBuf[hidx];
     }
     std::string hashBytes = ss.str();
     std::string baseSeqStr = string(baseSeq);
     hashBytes = baseSeqStr.substr(0, 8) + hashBytes + baseSeqStr.substr(baseSeqLen - 9, 8);
     
     return hashBytes;

}

char * LookupStickyFolderNameForSequence(const char *cfgFilePath, const char *baseSeqSpec) {
     if(cfgFilePath == NULL || baseSeqSpec == NULL) {
          return NULL;
     }
     off_t fnamePos = FolderNameForSequenceExists(cfgFilePath, baseSeqSpec);
     return LookupStickyFolderNameForSequence(cfgFilePath, fnamePos);
}

char * LookupStickyFolderNameForSequence(const char *cfgFilePath, RNAStructure *rnaStructSpec) {
     if(cfgFilePath == NULL || rnaStructSpec == NULL) {
          return NULL;
     }
     std::string baseSeq = std::string(rnaStructSpec->GetSequenceString());
     return LookupStickyFolderNameForSequence(cfgFilePath, baseSeq.c_str());
}

char * LookupStickyFolderNameForSequence(const char *cfgFilePath, off_t fnameFileOffset) {
     if(cfgFilePath == NULL || fnameFileOffset == LSEEK_NOT_FOUND) {
          return NULL;
     }
     cfgFilePath = GetStickyFolderConfigPath(cfgFilePath);
     FILE *fpCfgFile = fopen(cfgFilePath, "r+");
     if(!fpCfgFile) {
	  TerminalText::PrintError("Unable to open \"%s\" : %s\n", fpCfgFile, strerror(errno));
          return NULL;
     }
     int fdCfgFile = fileno(fpCfgFile);

     char lineBuf[MAX_BUFFER_SIZE];
     lseek(fdCfgFile, fnameFileOffset, SEEK_SET);
     fgets(lineBuf, MAX_BUFFER_SIZE, fpCfgFile);
     std::string stickyStrEntry = std::string(lineBuf);
     fclose(fpCfgFile);

     size_t firstQuotePos = stickyStrEntry.find_first_of("\"");
     size_t lastQuotePos = stickyStrEntry.find_last_of("\"");
     if(firstQuotePos == std::string::npos || lastQuotePos == std::string::npos) {
          return NULL;
     }
     std::string savedFolderName = stickyStrEntry.substr(firstQuotePos + 1, lastQuotePos - 1);
     char *rstr = (char *) malloc((savedFolderName.size() + 1) * sizeof(char));
     strcpy(rstr, savedFolderName.c_str());
     rstr[savedFolderName.size()] = '\0';
     return rstr;
     
}

off_t FolderNameForSequenceExists(const char *cfgFilePath, const char *baseSeqSpec) {
     if(cfgFilePath == NULL || baseSeqSpec == NULL) {
          return LSEEK_NOT_FOUND;
     }
     cfgFilePath = GetStickyFolderConfigPath(cfgFilePath);
     FILE *fpCfgFile = fopen(cfgFilePath, "r+");
     if(!fpCfgFile) {
          TerminalText::PrintError("Unable to open \"%s\" : %s\n", cfgFilePath, strerror(errno));
	  return LSEEK_NOT_FOUND;
     }
     int fdCfgFile = fileno(fpCfgFile);
     std::string baseSeqHash = HashBaseSequence(baseSeqSpec);
     char lineBuf[MAX_BUFFER_SIZE];
     off_t curFileOffset = 0;
     bool foundMatch = false;
     while(!feof(fpCfgFile) && fgets(lineBuf, MAX_BUFFER_SIZE, fpCfgFile)) {
          if(!strncmp(baseSeqHash.c_str(), lineBuf, BSHASH_BYTES)) {
	       foundMatch = true;
	       break;
	  }
	  curFileOffset = lseek(fdCfgFile, 0, SEEK_CUR);
     }
     fclose(fpCfgFile);
     if(foundMatch) {
          return curFileOffset;
     }
     return LSEEK_NOT_FOUND;
}

off_t FolderNameForSequenceExists(const char *cfgFilePath, RNAStructure *rnaStructSpec) {
     if(cfgFilePath == NULL || rnaStructSpec == NULL) {
          return LSEEK_NOT_FOUND;
     }
     std::string baseSeq = std::string(rnaStructSpec->GetSequenceString());
     return FolderNameForSequenceExists(cfgFilePath, baseSeq.c_str());
}

std::string ExtractSequenceNameFromButtonLabel(const char *buttonLabel) {
     std::string blabelStr = std::string(buttonLabel);
     size_t sepPos = blabelStr.find_first_of(FOLDER_NAME_DIVIDER);
     if(sepPos == std::string::npos) {
          return blabelStr;
     }
     return blabelStr.substr(sepPos + strlen(FOLDER_NAME_DIVIDER));
}

int SaveStickyFolderNameToConfigFile(const char *cfgFilePath, std::string baseSeq, 
		                     std::string folderName, off_t replacePos) {
     
     if(cfgFilePath == NULL) {
          return EINVAL;
     }
     cfgFilePath = GetStickyFolderConfigPath(cfgFilePath);
     
     FILE *fpCfgFile = fopen(cfgFilePath, "r+");
     if(!fpCfgFile) {
          TerminalText::PrintError("Unable to open \"%s\" : %s\n", cfgFilePath, strerror(errno));
	  return errno;
     }
     int fdCfgFile = fileno(fpCfgFile);
     char tempFilePath[MAX_BUFFER_SIZE];
     strcpy(tempFilePath, cfgFilePath);
     strcat(tempFilePath, ".temp");
     FILE *fpTempFile = fopen(tempFilePath, "w+");
     if(!fpTempFile) {
          TerminalText::PrintError("Unable to open \"%s\": %s\n", tempFilePath, strerror(errno));
	  fclose(fpCfgFile);
	  return errno;
     }
     int fdTempFile = fileno(fpTempFile);

     std::string saveFolderName = ExtractSequenceNameFromButtonLabel(folderName.c_str());
     if(saveFolderName.size() == 0) {
          return EINVAL;
     }
     std::string baseSeqHash = HashBaseSequence(baseSeq.c_str());

     char lineBuf[MAX_BUFFER_SIZE];
     off_t curCfgFileOffset = 0;
     while(!feof(fpCfgFile) && fgets(lineBuf, MAX_BUFFER_SIZE, fpCfgFile)) {
          off_t nextOffset = lseek(fdCfgFile, 0, SEEK_CUR);
	  if(replacePos == LSEEK_NOT_FOUND || nextOffset != replacePos) {
               fprintf(fpTempFile, "%s", lineBuf);
	  }
	  else {
               fprintf(fpTempFile, "%s;\"%s\"\n", baseSeqHash.c_str(), saveFolderName.c_str());
	  }
	  curCfgFileOffset = nextOffset;
     }
     fclose(fpCfgFile);
     fclose(fpTempFile);
     unlink(cfgFilePath);
     rename(tempFilePath, cfgFilePath);
     return EXIT_SUCCESS;

}

InputFileTypeSpec ClassifyInputFileTypeByExtension(const char *fileExt) {
     if(!strcasecmp(fileExt, "ct")) {
          return FILETYPE_CT;
     }
     else if(!strcasecmp(fileExt, "nopct")) {
	  return FILETYPE_NOPCT;
     }
     else if(!strcasecmp(fileExt, "dot") || !strcasecmp(fileExt, "bracket") || !strcasecmp(fileExt, "dbn")) {
	  return FILETYPE_DOTBRACKET;
     }
     else if(!strcasecmp(fileExt, "bpseq")) {
	  return FILETYPE_BPSEQ;
     }
     else if(!strcasecmp(fileExt, "gtb")) {
	  return FILETYPE_GTB;
     }
     else if(!strcasecmp(fileExt, "helix") || !strcasecmp(fileExt, "hlx")) {
	  return FILETYPE_HLXTRIPLE;
     }
     return FILETYPE_NONE;
}

InputFileTypeSpec ClassifyInputFileType(const char *inputFilePath) {
     if(inputFilePath == NULL) {
          return FILETYPE_NONE;
     }
     const char *extPos = strrchr(inputFilePath, '.');
     if(extPos == NULL) {
          return FILETYPE_NONE;
     }
     return ClassifyInputFileTypeByExtension(extPos + 1);
}

/* Reference for header comment stanadards: 
 * https://rna.urmc.rochester.edu/Text/File_Formats.html
 */
std::string GetSequenceFileHeaderLines(const char *filePath, InputFileTypeSpec fileType) {
     
     if(fileType == FILETYPE_NONE) {
          return "";
     }
     FILE *fpInputSeq = fopen(filePath, "r+");
     if(!fpInputSeq) {
          TerminalText::PrintError("Unable to open file \"%s\" : %s\n", filePath, strerror(errno));
	  return "";
     }
     std::string headerLines;
     char lineBuf[MAX_BUFFER_SIZE];
     bool addToHeaderStr = false, stopParsing = false;
     while(!feof(fpInputSeq) && fgets(lineBuf, MAX_BUFFER_SIZE, fpInputSeq)) {
          int lineLength = strlen(lineBuf);
	  if(lineLength == 0) {
	       stopParsing = true;
	       break;
	  }
	  else if(lineBuf[lineLength - 1] == '\n') {
	       lineBuf[lineLength - 1] = ' ';
	  }
          switch(fileType) {
               case FILETYPE_CT:
	       case FILETYPE_NOPCT:
	            if(!isspace(lineBuf[0]) && !isdigit(lineBuf[0])) {
		         addToHeaderStr = true;
	            }
		    else {
		         stopParsing = true;
	            }
		    break;
	       case FILETYPE_BPSEQ:
	            if(lineBuf[0] != ';') {
		         addToHeaderStr = true;
			 stopParsing = true;
	            }
	            break;
	       case FILETYPE_DOTBRACKET:
	       case FILETYPE_GTB:
	       case FILETYPE_HLXTRIPLE:
	            if(lineBuf[0] == '>') {
		         addToHeaderStr = true;
	            }
		    else {
		         stopParsing = true;
	            }
		    break;
	       default:
	            stopParsing = true;
	            break;
	  }
	  if(addToHeaderStr) {
	       headerLines += string(lineBuf);
	  }
	  if(stopParsing) {
	       break;
	  }
	  addToHeaderStr = false;
     }
     fclose(fpInputSeq);
     return headerLines;

}

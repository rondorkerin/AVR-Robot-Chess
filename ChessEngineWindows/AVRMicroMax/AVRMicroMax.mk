##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AVRMicroMax
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "C:\Users\nul4x3r0000\Dropbox\Programming\AVR-Robot-Chess\ChessEngineWindows\AVRMicroMax"
ProjectPath            := "C:\Users\nul4x3r0000\Dropbox\Programming\AVR-Robot-Chess\ChessEngineWindows\AVRMicroMax"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=nul4x3r0000
Date                   :=8/12/2012
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=gcc
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=gcc
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="C:\Users\nul4x3r0000\Dropbox\Programming\AVR-Robot-Chess\ChessEngineWindows\AVRMicroMax\AVRMicroMax.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
CmpOptions             := -g -O0 -Wall $(Preprocessors)
C_CmpOptions           := -g -O0 -Wall $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects=$(IntermediateDirectory)/chessengine$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/chessengine$(ObjectSuffix): chessengine.c $(IntermediateDirectory)/chessengine$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/nul4x3r0000/Dropbox/Programming/AVR-Robot-Chess/ChessEngineWindows/AVRMicroMax/chessengine.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/chessengine$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/chessengine$(DependSuffix): chessengine.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/chessengine$(ObjectSuffix) -MF$(IntermediateDirectory)/chessengine$(DependSuffix) -MM "C:/Users/nul4x3r0000/Dropbox/Programming/AVR-Robot-Chess/ChessEngineWindows/AVRMicroMax/chessengine.c"

$(IntermediateDirectory)/chessengine$(PreprocessSuffix): chessengine.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/chessengine$(PreprocessSuffix) "C:/Users/nul4x3r0000/Dropbox/Programming/AVR-Robot-Chess/ChessEngineWindows/AVRMicroMax/chessengine.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/chessengine$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/chessengine$(DependSuffix)
	$(RM) $(IntermediateDirectory)/chessengine$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "C:\Users\nul4x3r0000\Dropbox\Programming\AVR-Robot-Chess\ChessEngineWindows\AVRMicroMax\.build-debug\AVRMicroMax"



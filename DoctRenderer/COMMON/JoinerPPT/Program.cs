﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Joiner
{
    class Program
    {
        static void Main(string[] args)
        {
            string strApplication = Directory.GetCurrentDirectory();
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);

            string strRoot = strApplication + "\\OfficeWeb\\";
            List<string> files = new List<string>();

            string[] arrFilesConfig = {
                "Common/browser.js",
                "Common/docscoapisettings.js",
                "Common/docscoapicommon.js",
                "Common/docscoapi.js",
                "Common/wordcopypaste.js",
                "Common/downloaderfiles.js",
                "Common/editorscommon.js",
                "Common/apiCommon.js",
                "PowerPoint/Editor/Serialize.js",
                "Common/Shapes/SerializeWriter.js",
                "Common/FontsFreeType/font_engine.js",
                "Common/FontsFreeType/FontFile.js",
                "Common/FontsFreeType/FontManager.js",
                "Word/Drawing/Externals.js",
                "Word/Drawing/GlobalLoaders.js",
                "PowerPoint/Drawing/ThemeLoader.js",
                "PowerPoint/Editor/Format/CollaborativeEditing.js",
                "PowerPoint/Editor/Styles.js",
                "PowerPoint/Editor/Format/ParagraphContent.js",
                "PowerPoint/Editor/Format/FontClassification.js",
                "PowerPoint/Editor/Format/Spelling.js",
                "PowerPoint/Editor/Format/Comments.js",
                "PowerPoint/Editor/GraphicObjects.js",
                "PowerPoint/Editor/States.js",
                "Common/Charts/DrawingObjects.js",
                "Common/commonDefines.js",
                "Common/SerializeCommonWordExcel.js",
				"Common/SerializeChart.js",
                "Word/Editor/Serialize2.js",
                "Common/Charts/charts.js",
                "Common/trackFile.js",
                "Word/Editor/SerializeCommon.js",
                "Word/Drawing/Metafile.js",
                "Word/Drawing/GraphicsEvents.js",
                "Word/Drawing/WorkEvents.js",
                "Word/Drawing/Controls.js",
                "Word/Drawing/Rulers.js",
                "PowerPoint/Editor/Format/Table.js",
                "Word/Editor/Common.js",
                "Word/Editor/Sections.js",
                "Common/scroll.js",
                "PowerPoint/Editor/History.js",
                "PowerPoint/Editor/Format/Paragraph.js",
                "PowerPoint/Editor/FlowObjects.js",
                "PowerPoint/Editor/Format/DocumentContent.js",
                "PowerPoint/Editor/Format/Presentation.js",
                "PowerPoint/Editor/SlideShowInfo.js",
                "PowerPoint/Editor/PresentationProperties.js",
                "PowerPoint/Editor/Format/Shape.js",
                "PowerPoint/Editor/Format/GraphicFrame.js",
                "PowerPoint/Editor/Numbering.js",
                "PowerPoint/Editor/TrackObjects/AdjustmentTracks.js",
                "PowerPoint/Editor/TrackObjects/MoveTracks.js",
                "PowerPoint/Editor/TrackObjects/NewShapeTracks.js",
                "PowerPoint/Editor/TrackObjects/PolyLine.js",
                "PowerPoint/Editor/TrackObjects/ResizeTracks.js",
                "PowerPoint/Editor/TrackObjects/RotateTracks.js",
                "PowerPoint/Editor/TrackObjects/Spline.js",
                "PowerPoint/Editor/Format/TextBody.js",
                "PowerPoint/Editor/Format/Image.js",
                "PowerPoint/Editor/Format/GroupShape.js",
                "PowerPoint/Editor/Format/ChartTitle.js",
                "PowerPoint/Editor/Format/Chart.js",
                "PowerPoint/Editor/Format/ChartLayout.js",
                "PowerPoint/Editor/Format/Slide.js",
                "PowerPoint/Editor/Format/SlideMaster.js",
                "PowerPoint/Editor/Format/Layout.js",
                "PowerPoint/Editor/Drawing/Geometry.js",
                "PowerPoint/Editor/Drawing/CreateGoemetry.js",
                "PowerPoint/Editor/Drawing/ColorArray.js",
                "PowerPoint/Editor/Drawing/Math.js",
                "PowerPoint/Editor/Drawing/Path.js",
                "PowerPoint/Editor/Drawing/ArcTo.js",
                "PowerPoint/Editor/Drawing/GraphicFigure.js",
                "PowerPoint/Editor/Drawing/Clone.js",
                "PowerPoint/Editor/Drawing/CImage.js",
                "PowerPoint/Editor/Drawing/Spline.js",
                "PowerPoint/Editor/Drawing/PolyLine.js",
                "PowerPoint/Editor/Drawing/Hit.js",
                "PowerPoint/Editor/Drawing/Joined.js",
                "Word/Drawing/Graphics.js",
                "Word/Drawing/Overlay.js",
                "Word/Drawing/HatchPattern.js",
                "Word/Drawing/ShapeDrawer.js",
                "PowerPoint/Drawing/Transitions.js",
                "PowerPoint/Drawing/DrawingDocument.js",
                "PowerPoint/Drawing/HtmlPage.js",
                "PowerPoint/Editor/Format.js",
                "PowerPoint/apiDefines.js",
                "PowerPoint/api.js",
                "PowerPoint/apiCommon.js",

                "Common/Shapes/EditorSettings.js",
                "PowerPoint/themes/Themes.js"
            };

            if (true)
            {
                files.Clear();
                for (int i = 0; i < arrFilesConfig.Length; ++i)
                {
                    files.Add(arrFilesConfig[i]);
                }
            }

            StringBuilder oBuilder = new StringBuilder();

            for (int i = 0; i < files.Count; i++)
            {
                StreamReader oReader = new StreamReader(strRoot + files[i]);
                oBuilder.Append(oReader.ReadToEnd());

                oBuilder.Append("\n\n");
            }

            string strDestPath = strApplication + "\\OfficeWeb\\PowerPoint\\sdk-all.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}

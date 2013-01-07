//------------------------------------------------------------------------------------------------
// Copyright (c) 2012  Alessandro Bria and Giulio Iannello (University Campus Bio-Medico of Rome).  
// All rights reserved.
//------------------------------------------------------------------------------------------------

/*******************************************************************************************************************************************************************************************
*    LICENSE NOTICE
********************************************************************************************************************************************************************************************
*    By downloading/using/running/editing/changing any portion of codes in this package you agree to this license. If you do not agree to this license, do not download/use/run/edit/change
*    this code.
********************************************************************************************************************************************************************************************
*    1. This material is free for non-profit research, but needs a special license for any commercial purpose. Please contact Alessandro Bria at a.bria@unicas.it or Giulio Iannello at 
*       g.iannello@unicampus.it for further details.
*    2. You agree to appropriately cite this work in your related studies and publications.
*
*       Bria, A., et al., (2012) "Stitching Terabyte-sized 3D Images Acquired in Confocal Ultramicroscopy", Proceedings of the 9th IEEE International Symposium on Biomedical Imaging.
*       Bria, A., Iannello, G., "A Tool for Fast 3D Automatic Stitching of Teravoxel-sized Datasets", submitted on July 2012 to IEEE Transactions on Information Technology in Biomedicine.
*
*    3. This material is provided by  the copyright holders (Alessandro Bria  and  Giulio Iannello),  University Campus Bio-Medico and contributors "as is" and any express or implied war-
*       ranties, including, but  not limited to,  any implied warranties  of merchantability,  non-infringement, or fitness for a particular purpose are  disclaimed. In no event shall the
*       copyright owners, University Campus Bio-Medico, or contributors be liable for any direct, indirect, incidental, special, exemplary, or  consequential  damages  (including, but not 
*       limited to, procurement of substitute goods or services; loss of use, data, or profits;reasonable royalties; or business interruption) however caused  and on any theory of liabil-
*       ity, whether in contract, strict liability, or tort  (including negligence or otherwise) arising in any way out of the use of this software,  even if advised of the possibility of
*       such damage.
*    4. Neither the name of University  Campus Bio-Medico of Rome, nor Alessandro Bria and Giulio Iannello, may be used to endorse or  promote products  derived from this software without
*       specific prior written permission.
********************************************************************************************************************************************************************************************/

#ifndef PCONVETER_GUI_H
#define PCONVETER_GUI_H

#include <QtGui>
#include <v3d_interface.h>
#include "control/CPlugin.h"

class teramanager::PConverter : public QWidget
{
    Q_OBJECT

    private:

        /*********************************************************************************
        * Singleton design pattern: this class can have one instance only,  which must be
        * instantiated by calling static method "istance(...)"
        **********************************************************************************/
        static PConverter* uniqueInstance;
        PConverter(){}
        PConverter(V3DPluginCallback *callback, QWidget *parent);

        //members
        V3DPluginCallback* V3D_env;     //handle of V3D environment
        QWidget *parentWidget;          //handle of parent widget

        //main widgets
        QGroupBox* import_panel;
        QGroupBox* conversion_panel;
        QProgressBar* progressBar;      //progress bar
        QPushButton* startButton;       //start button
        QPushButton* stopButton;        //stop button
        QStatusBar* statusBar;          //status bar

        //import form widget
        QComboBox* volformatCombobox;   //combobox for volume format selection
        QLabel* helpIcon;
        QLabel* volformatHelpbox;       //help box describing the selected volume format
        QLineEdit *volpathField;        //field for either volume's directory or file path
        QPushButton *voldirButton;      //browse for volume's directory button
        QPushButton *volfileButton;     //browse for volume's file button
        QStackedLayout* volButtonLayout;//stacked layout used to show only one of the two buttons

        //conversion form widget
        QPushButton *voloutdirButton;      //browse for volume's output directory button
        QLineEdit *voloutpathField;        //field for volume's output directory path
        QGridLayout* resolutionsLayout; //layout containing resolutions dynamically added after volume import
        int resolutionsNumber;          //number of selectable resolutions (it is detected at runtime)
        QLabel** resolutionsFields;     //dynamic array of resolution fields
        QLabel** resolutionsSizes;      //dynamic array of resolution size fields
        QCheckBox** resolutionsCboxs;   //dynamic array of resolution comboboxes
        QSpinBox* stacksWidthField;     //field to select stacks width
        QSpinBox* stacksHeightField;    //field to select stacks height
        QLabel* memoryField;            //field for memory usage estimation

        //friend class declarations
        friend class CConverter;

        //timer used to flush <statusBar> message
        QTimer* timer;
        string timerFlushingMessage;
        int timerTransitions;


    public:

        /*********************************************************************************
        * Singleton design pattern: this class can have one instance only,  which must be
        * instantiated by calling static method "istance(...)"
        **********************************************************************************/
        static PConverter* instance(V3DPluginCallback *callback=0, QWidget *parent=0);
        static void uninstance();
        ~PConverter();

        //GET and SET methods
        V3DPluginCallback* getV3D_env(){return V3D_env;}
        QProgressBar* getProgressBar(){return progressBar;}
        QStatusBar* getStatusBar(){return statusBar;}
        void setStartButtonEnabled(bool enabled){startButton->setEnabled(enabled);}
        void setStopButtonEnabled(bool enabled){stopButton->setEnabled(enabled);}

        //resets progress bar, start/stop buttons and tab bar
        void resetGUI();

        //overrides closeEvent method of QWidget
        void closeEvent(QCloseEvent *evt);

        /**********************************************************************************
        * Called by algorithms running from different threads.
        * Emits <sendProgressBarChanged> signal
        ***********************************************************************************/
        void emitProgressBarChanged(int val, int minutes, int seconds, const char* message = 0)
        {emit sendProgressBarChanged(val, minutes, seconds, message);}

    public slots:

        void startButtonClicked();
        void stopButtonClicked();
        void voldirButtonClicked();
        void volfileButtonClicked();
        void voldiroutButtonClicked();
        void volformatChanged ( const QString & text );

        /**********************************************************************************
        * <sendProgressBarChanged> event handler
        ***********************************************************************************/
        void progressBarChanged(int val, int minutes, int seconds, const char* message);

        /**********************************************************************************
        * Called by <CConverter> when the associated operation has been performed.
        * If an exception has occurred in the <CConverter> thread, it is propagated and
        * managed in the current thread (ex != 0).
        ***********************************************************************************/
        void operationDone(MyException *ex);

        /**********************************************************************************
        * Called when the GUI widgets that control application settings change.
        * This is used to manage persistent platform-independent application settings.
        ***********************************************************************************/
        void settingsChanged();

        /**********************************************************************************
        * Updates widgets contents after user interaction
        ***********************************************************************************/
        void updateContent();

        /**********************************************************************************
        * Used to flush <statusBar> text
        ***********************************************************************************/
        void timerFired();

    signals:

        /*********************************************************************************
        * Carries progress bar informations (progress percentage and remaining minutes).
        **********************************************************************************/
        void sendProgressBarChanged(int val, int minutes, int seconds, const char* message);
};

#endif // PCONVETER_GUI_H

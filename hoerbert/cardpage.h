/***************************************************************************
 * hörbert Software
 * Copyright (C) 2019 WINZKI GmbH & Co. KG
 *
 * Authors of the original version: Igor Yalovenko, Rainer Brang
 * Dec. 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef CARDPAGE_H
#define CARDPAGE_H

#include <QWidget>

#if defined (Q_OS_MACOS) || defined (Q_OS_LINUX)
#include <QFileSystemWatcher>
#endif

#include <list>

#include "devicemanager.h"
#include "define.h"

#ifdef Q_OS_WIN
#include "../libQDeviceWatcher/qdevicewatcher.h"
#endif

const int GRID_SPACING              = 16;
const int MOUNT_VOLUME_DELAY        = 2; // in seconds

class QPushButton;
class PieButton;
class QComboBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QStackedWidget;

class DeviceManager;
class PieButton;
class CapacityBar;

typedef std::shared_ptr<DeviceManager> DeviceManager_ptr;

/**
 * @brief The CardPage class represents a page to deal with memory cards and to show directories in them
 */
class CardPage : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief CardPage constructor
     * @param parent
     */
    explicit CardPage(QWidget *parent = Q_NULLPTR);

    /**
     * @brief CardPage destructor
     */
    ~CardPage();

    enum DIRECTORIES {
        DIR0 = 0,
        DIR1,
        DIR2,
        DIR3,
        DIR4,
        DIR5,
        DIR6,
        DIR7,
        DIR8
    };

    /**
     * @brief format a memory card
     */
    void formatSelectedDrive(bool retry = false);

    /**
     * @brief eject a memory card
     */
    void ejectDrive();

    /**
     * @brief recreate hoerbert.xml
     */
    void recreateXml();

    /**
     * @brief select a memory card to work on
     */
    void selectDrive();

    /**
     * @brief get the color for a given directory number
     * @param id
     * @return color of the given directory number
     */
    QColor getDirectoryColor(quint8 id);

    /**
     * @brief update
     */
    void update();

    /**
     * @brief set the percent value for the pie chart (completion percent)
     * @param buttonIndex the button to set
     * @param percentage the percentage to set
     */
    void setPercent(int buttonIndex, int percentage);

    /**
     * @brief set the enabled state of a button
     * @param buttonIndex the button to set
     * @param flag true=enabled, false=disabled
     */
    void setButtonEnabled(int buttonIndex, bool flag);

    /**
     * @brief absolute path of selected card
     */
    QString currentDrivePath();

    /**
     * @brief label of selected drive
     */
    QString currentDriveName();

    /**
     * @brief set whether the app is processing or not
     */
    void setIsProcessing(bool flag);

    /**
     * @brief isProcessing
     * @return true when there's commiting on progress, otherwise false
     */
    bool isProcessing();


    /**
     * @brief set whether hoerbertXML needs to be rewritten
     */
    void setHoerbertXMLDirty( bool yesNo );

    /**
     * @brief isHoerbertXMLDirty
     * @return true when there has been some write action of any kind on the playlists
     */
    bool isHoerbertXMLDirty();

    /**
     * @brief update the numbers: space used and space available
     */
    void updateUsedSpace();

    /**
     * @brief enable/disable buttons for card management on the page
     * @param flag flag
     */
    void setCardManageButtonsEnabled(bool flag);

    /**
     * @brief switchDiagnosticsMode
     * @param enabled true when diagnostics mode is enabled
     */
    void switchDiagnosticsMode(bool enabled);

    /**
     * @brief isDiagnosticsModeEnabled
     * @return
     */
    bool isDiagnosticsModeEnabled();

    /**
     * @brief get volume size of current memory card
     * @return
     */
    qint64 getCurrentVolumeSize();

    /**
     * @brief get available space of current memory card
     * @return
     */
    qint64 getCurrentAvailableSpace();

    /**
     * @brief get filesystem type of memory card
     * @return
     */
    QString getCardFileSystemType();

    /**
     * @brief select drive by its full path on file system
     * @param path absolute path to the drive storage
     */
    void selectDriveByPath(const QString &path);

signals:

    /**
     * @brief this signal is emitted when the directory is selected
     * @param id directory identifier
     * @param drivePath absolute path to the directory on the drive
     * @param audioList list of audio info in the directory
     */
    void directoryChanged(qint8 id, const QString &drivePath, const AudioList &audioList);

    /**
     * @brief this signal is emitted when there's change in drive space
     * @param used bytes
     * @param total bytes
     */
    void driveCapacityUpdated(quint64 used, quint64 total);

    /**
     * @brief this signal is emitted when a drive selected for further edit
     */
    void driveSelected(const QString &);

    /**
     * @brief this signal is emitted when user selects a memory card which is used with old software last time
     */
    void migrationNeeded(const QString &dirPath);

    /**
     * @brief this signal is emitted when user switches diagnostics mode
     * @param enabled
     */
    void diagnosticsModeSwitched(bool enabled = false);

    /**
     * @brief this signal is emitted when the memory card has plausibility issues
     * @param fixList
     */
    void plausibilityFixNeeded(std::list <int> fixList);

public slots:

    /**
     * @brief updateDriveList updates list of currently available devices
     */
    void updateDriveList();

private slots:

    /**
     * @brief called when a directory button is clicked
     * @param dirIndex index of directory(equals to actual directory name)
     */
    void onDirectoryClicked(qint8 dirIndex);

    /**
     * @brief called when a removable device is added
     */
    void onDeviceAdded(const QString &);

    /**
     * @brief called when a removable device is removed
     */
    void onDeviceRemoved(const QString &);

private:

    /**
     * @brief initialize directory buttons
     */
    void initializeDirectories();

    /**
     * @brief read the drive and shows directories
     * @param driveName drive label + root path
     */
    void selectDrive(const QString &driveName);

    /**
     * @brief initialize directory buttons and hide directory details
     */
    void deselectDrive();

    bool m_isProcessing;
    bool m_migrationSuggested;
    bool m_hoerbertXMLIsDirty;

    DeviceManager_ptr m_deviceManager;
#ifdef Q_OS_WIN
    QDeviceWatcher *m_deviceWatcher;
#else
    QFileSystemWatcher m_watcher;
#endif

    QWidget *m_cardMngContainer;
    QComboBox *m_driveList;
    QPushButton *m_selectDriveButton;
    PieButton *m_ejectDriveButton;
    QLabel *m_ejectButtonLabel;
    QHBoxLayout *m_cardMngLayout;
    QVBoxLayout *m_mainLayout;

    QStackedWidget *m_stackWidget;

    QWidget *m_gridWidget;
    QGridLayout *m_gridLayout;

    QWidget *m_diagWidget;
    QVBoxLayout *m_diagLayout;
    QLabel *m_diagModeHint;
    QPushButton *m_return2Normal;

    PieButton *m_dir0;
    PieButton *m_dir1;
    PieButton *m_dir2;
    PieButton *m_dir3;
    PieButton *m_dir4;
    PieButton *m_dir5;
    PieButton *m_dir6;
    PieButton *m_dir7;
    PieButton *m_dir8;
    QMap<int, PieButton*> m_dirs;

    QSpacerItem *m_horizontalGridSpacer1;
    QSpacerItem *m_horizontalGridSpacer2;
    QSpacerItem *m_verticalGridSpacer1;
    QSpacerItem *m_verticalGridSpacer2;
    QSpacerItem *m_leftGridSpacer;
    QSpacerItem *m_rightGridSpacer;
};

#endif // CARDPAGE_H

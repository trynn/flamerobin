/*
  Copyright (c) 2004-2007 The FlameRobin Development Team

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


  $Id$

*/

#ifndef EXECUTESQLFRAME_H
#define EXECUTESQLFRAME_H
//-----------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

#include <ibpp.h>

#include "core/Observer.h"
#include "core/StringUtils.h"
#include "gui/BaseFrame.h"
#include "gui/FindDialog.h"
#include "logger.h"
#include "sql/SqlStatement.h"
#include "metadata/database.h"
#include "statementHistory.h"
//-----------------------------------------------------------------------------
class DataGrid;
class ExecuteSqlFrame;
//-----------------------------------------------------------------------------
class SqlEditor: public SearchableEditor
{
private:
    void setup();
public:
    SqlEditor(wxWindow *parent, wxWindowID id);
    void markText(int start, int end);
    void setFont();

    void OnContextMenu(wxContextMenuEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    DECLARE_EVENT_TABLE()
};
//-----------------------------------------------------------------------------
class ExecuteSqlFrame: public BaseFrame, public Observer
{
public:
    Database *getDatabase();
    void showProperties(wxString objectName);
    // query parsing and execution
    void executeAllStatements(bool autoExecute = false);
    void prepareAndExecute(bool prepareOnly = false);
    bool parseStatements(const wxString& statements, bool autoExecute = false,
        bool prepareOnly = false, int selectionOffset = 0);
    bool execute(wxString sql, const wxString& terminator,
        bool prepareOnly = false);
    bool loadSqlFile(const wxString& filename);
    void setSql(wxString sql);
    void clearStats();
    void compareCounts(IBPP::DatabaseCounts& one,
        IBPP::DatabaseCounts& two);

    ExecuteSqlFrame(wxWindow* parent, int id, wxString title, Database *db,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

    enum {
        ID_grid_data = 101,
        ID_stc_sql
    };

private:
    std::vector<SqlStatement> executedStatementsM;
    wxString filenameM;

    typedef enum { ttNormal, ttSql, ttError } TextType;
    void log(wxString s, TextType type = ttNormal);     // write messages to textbox
    void SplitScreen();
    Database* databaseM;
    DatabaseToSystemCharsetConversion dbCharsetConversionM;
    void setDatabase(Database* db);

    StatementHistory::Position historyPositionM;
    wxString localBuffer;

    bool autoCommitM;
    bool inTransactionM;
    IBPP::Transaction transactionM;
    IBPP::Statement statementM;
    void InTransaction(bool started);       // changes controls (enable/disable)
    bool commitTransaction();
    void rollbackTransaction();

    void autoComplete(bool force);
    void autoCompleteColumns(int pos, int len = 0);
    void OnSqlEditUpdateUI(wxStyledTextEvent &event);
    void OnSqlEditCharAdded(wxStyledTextEvent &event);      // autocomplete stuff
    void OnSqlEditStartDrag(wxStyledTextEvent& event);      // enable click&remove selection
    wxString keywordsM;     // text used for autocomplete
    void setKeywords();
    void buildMainMenu();
    void buildToolbar();

    // events
    void OnClose(wxCloseEvent& event);
    void OnKeyDown(wxKeyEvent &event);
    void OnGridRowCountChanged(wxCommandEvent &event);
    void OnGridStatementExecuted(wxCommandEvent &event);

    // menu events
    void OnMenuNew(wxCommandEvent &event);
    void OnMenuOpen(wxCommandEvent &event);
    void OnMenuSave(wxCommandEvent &event);
    void OnMenuSaveAs(wxCommandEvent &event);
    void OnMenuClose(wxCommandEvent &event);

    void OnMenuUndo(wxCommandEvent &event);
    void OnMenuUpdateUndo(wxUpdateUIEvent& event);
    void OnMenuRedo(wxCommandEvent &event);
    void OnMenuUpdateRedo(wxUpdateUIEvent& event);
    void OnMenuCut(wxCommandEvent &event);
    void OnMenuUpdateCut(wxUpdateUIEvent& event);
    void OnMenuCopy(wxCommandEvent &event);
    void OnMenuUpdateCopy(wxUpdateUIEvent& event);
    void OnMenuPaste(wxCommandEvent &event);
    void OnMenuUpdatePaste(wxUpdateUIEvent& event);
    void OnMenuDelete(wxCommandEvent &event);
    void OnMenuUpdateDelete(wxUpdateUIEvent& event);
    void OnMenuSelectAll(wxCommandEvent &event);
    void OnMenuReplace(wxCommandEvent &event);

    void OnMenuUnsplitView(wxCommandEvent &event);
    void OnMenuSplitView(wxCommandEvent &event);
    void OnMenuSetEditorFont(wxCommandEvent &event);
    void OnMenuToggleWrap(wxCommandEvent &event);
    void OnMenuFocusEditor(wxCommandEvent &event);
    void OnMenuFocusGrid(wxCommandEvent &event);
    void OnMenuToggleClick(wxCommandEvent &event);  // toolbar

    void OnMenuHistoryNext(wxCommandEvent &event);
    void OnMenuUpdateHistoryNext(wxUpdateUIEvent& event);
    void OnMenuHistoryPrev(wxCommandEvent &event);
    void OnMenuUpdateHistoryPrev(wxUpdateUIEvent& event);
    void OnMenuHistorySearch(wxCommandEvent &event);

    void OnMenuExecute(wxCommandEvent &event);
    void OnMenuShowPlan(wxCommandEvent &event);
    void OnMenuExecuteSelection(wxCommandEvent &event);
    void OnMenuExecuteFromCursor(wxCommandEvent &event);
    void OnMenuCommit(wxCommandEvent &event);
    void OnMenuRollback(wxCommandEvent &event);
    void OnMenuUpdateExecuteSelection(wxUpdateUIEvent& event);
    void OnMenuUpdateWhenInTransaction(wxUpdateUIEvent& event);
    void OnMenuUpdateWhenExecutePossible(wxUpdateUIEvent& event);

    void OnMenuGridInsertRow(wxCommandEvent &event);
    void OnMenuUpdateGridInsertRow(wxUpdateUIEvent& event);
    void OnMenuGridDeleteRow(wxCommandEvent &event);
    void OnMenuUpdateGridDeleteRow(wxUpdateUIEvent& event);
    void OnMenuGridCopy(wxCommandEvent &event);
    void OnMenuGridCopyAsInsert(wxCommandEvent &event);
    void OnMenuGridCopyAsUpdate(wxCommandEvent &event);
    void OnMenuGridSaveAsHtml(wxCommandEvent &event);
    void OnMenuGridSaveAsCsv(wxCommandEvent &event);
    void OnMenuGridGridHeaderFont(wxCommandEvent &event);
    void OnMenuGridGridCellFont(wxCommandEvent &event);
    void OnMenuGridFetchAll(wxCommandEvent &event);
    void OnMenuGridCancelFetchAll(wxCommandEvent &event);
    void OnMenuUpdateGridHasSelection(wxUpdateUIEvent& event);
    void OnMenuUpdateGridFetchAll(wxUpdateUIEvent& event);
    void OnMenuUpdateGridCancelFetchAll(wxUpdateUIEvent& event);

    void OnMenuFindSelectedObject(wxCommandEvent& event);

    // begin wxGlade: ExecuteSqlFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    void removeSubject(Subject* subject);
    virtual void update();

    bool closeWhenTransactionDoneM;
    bool loadingM;

    wxPanel* panel_contents;

    wxSplitterWindow* splitter_window_1;
    SqlEditor* styled_text_ctrl_sql;
    wxNotebook* notebook_1;
    wxPanel* notebook_pane_1;
    wxPanel* notebook_pane_2;
    DataGrid* grid_data;
    wxStyledTextCtrl* styled_text_ctrl_stats;

    wxStatusBar* statusbar_1;

    wxMenuBar* menuBarM;
    wxToolBar* toolBarM;

    virtual const wxString getName() const;
    virtual void doReadConfigSettings(const wxString& prefix);
    virtual void doWriteConfigSettings(const wxString& prefix) const;
    virtual const wxRect getDefaultRect() const;

    DECLARE_EVENT_TABLE()
};
//-----------------------------------------------------------------------------
#endif // EXECUTESQLFRAME_H

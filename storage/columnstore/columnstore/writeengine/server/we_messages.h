/* Copyright (C) 2014 InfiniDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

/*******************************************************************************
 * $Id: we_messages.h 4609 2013-04-19 15:32:02Z chao $
 *
 *******************************************************************************/
#ifndef WE_MESSAGES_H__
#define WE_MESSAGES_H__

namespace WriteEngine
{
enum ServerMessages
{
  WE_SVR_LOOPBACK,
  WE_SVR_DDL_KEEPALIVE,
  WE_SVR_DML_KEEPALIVE,
  WE_UPDATE_NEXTVAL,
  WE_SVR_WRITE_SYSTABLE,
  WE_SVR_WRITE_SYSCOLUMN,
  WE_SVR_WRITE_CREATETABLEFILES,
  WE_SVR_COMMIT_VERSION,
  WE_SVR_ROLLBACK_BLOCKS,
  WE_SVR_ROLLBACK_VERSION,
  WE_SVR_DELETE_SYSTABLE,
  WE_SVR_DELETE_SYSTABLES,
  WE_SVR_DELETE_SYSCOLUMN,
  WE_SVR_DELETE_SYSCOLUMN_ROW,
  WE_SVR_WRITE_DROPFILES,
  WE_SVR_UPDATE_SYSTABLE_AUTO,
  WE_SVR_UPDATE_SYSCOLUMN_NEXTVAL,
  WE_SVR_UPDATE_SYSCOLUMN_TABLENAME,
  WE_SVR_UPDATE_SYSCOLUMN_AUTO,
  WE_SVR_UPDATE_SYSCOLUMN_AUTOVAL,
  WE_SVR_UPDATE_SYSCOLUMN_COLPOS,
  WE_SVR_UPDATE_SYSCOLUMN_RENAMECOLUMN,
  WE_SVR_UPDATE_SYSTABLE_TABLENAME,
  WE_SVR_UPDATE_SYSTABLES_TABLENAME,
  WE_SVR_DROP_PARTITIONS,
  WE_SVR_SINGLE_INSERT,
  WE_SVR_BATCH_KEEPALIVE,
  WE_SVR_BATCH_INSERT,
  WE_SVR_BATCH_INSERT_END,
  WE_SVR_COMMIT_BATCH_AUTO_ON,
  WE_SVR_ROLLBACK_BATCH_AUTO_ON,
  WE_SVR_COMMIT_BATCH_AUTO_OFF,
  WE_SVR_ROLLBACK_BATCH_AUTO_OFF,
  WE_SVR_BATCH_AUTOON_REMOVE_META,
  WE_SVR_UPDATE,
  WE_SVR_FLUSH_FILES,  // 35
  WE_SVR_DELETE,
  WE_SVR_DML_BULKROLLBACK,
  WE_SVR_DML_BULKROLLBACK_CLEANUP,
  WE_SVR_FILL_COLUMN,
  WE_SVR_WRITE_TRUNCATE,
  WE_SVR_WRITE_DROPPARTITION,
  WE_SVR_WRITE_DROPTABLE,
  WE_SVR_DELETE_DDLLOG,
  WE_SVR_FETCH_DDL_LOGS,
  WE_SVR_REMOVE_TABLEDATA,
  WE_SVR_UPDATE_SYSCOLUMN_DEFAULTVAL,
  WE_SVR_REDISTRIBUTE,
  WE_SVR_CLOSE_CONNECTION,
  WE_SVR_GET_FILESIZES,
  WE_SVR_GET_FILESIZE,
  WE_SVR_PURGEFD,
  WE_END_TRANSACTION,
  WE_SRV_FIX_ROWS,
  WE_SVR_WRITE_CREATE_SYSCOLUMN,
  WE_SVR_BATCH_INSERT_BINARY,
  WE_SVR_GET_WRITTEN_LBIDS,

  WE_CLT_SRV_DATA = 100,
  WE_CLT_SRV_EOD,
  WE_CLT_SRV_CMD,
  WE_CLT_SRV_ACK,
  WE_CLT_SRV_NAK,
  WE_CLT_SRV_PM_ERROR,
  WE_CLT_SRV_KEEPALIVE,
  WE_CLT_SRV_IMPFILENAME,
  WE_CLT_SRV_IMPFILEERROR,
  WE_CLT_SRV_CMDLINEARGS,
  WE_CLT_SRV_STARTCPI,
  WE_CLT_SRV_CPIPASS,
  WE_CLT_SRV_CPIFAIL,
  WE_CLT_SRV_ROLLBACK,
  WE_CLT_SRV_CLEANUP,
  WE_CLT_SRV_DATARQST,
  WE_CLT_SRV_MODE,
  WE_CLT_SRV_DBRCNT,
  WE_CLT_SRV_BRMRPT,
  WE_CLT_SRV_JOBID,
  WE_CLT_SRV_JOBDATA,
  WE_CLT_SRV_ERRLOG,
  WE_CLT_SRV_BADLOG,
  WE_CLT_SRV_CLEAR_TABLE_LOCK,
  WE_CLT_SRV_CLEAR_TABLE_LOCK_CLEANUP

};

}

#endif

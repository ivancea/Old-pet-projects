using System;
using System.Collections.Generic;
using System.Data;
using MySql.Data.MySqlClient;

namespace MtgDatabaseClient {
    class DataConnection {
        public static string server = "",
                             userid = "",
                             password = "",
                             database = "";

        private string lastError;
        private int lastErrorCode;

        private long lastInsertedId;

        private MySqlConnection conn;

        public DataConnection() {
            initialize();
        }

        private void initialize() {
            string connectionString = String.Format("server={0};userid={1};password={2};database={3};Convert Zero Datetime=True",
                                                    server, userid, password, database);
            conn = new MySqlConnection(connectionString);
        }

        public bool testConnection() {
            bool ret = true;
            try {
                conn.Open();
            } catch (MySqlException e) {
                lastError = e.Message;
                lastErrorCode = e.Number;
                ret = false;
            } finally {
                if (conn.State == ConnectionState.Open)
                    conn.Close();
            }
            return ret;
        }

        private bool openConnection() {
            try {
                conn.Open();
                return true;
            } catch (MySqlException e) {
                lastError = e.Message;
                lastErrorCode = e.Number;
                return false;
            }
        }

        private bool closeConnection() {
            try {
                conn.Close();
                return true;
            } catch (MySqlException e) {
                lastError = e.Message;
                lastErrorCode = e.Number;
                return false;
            }
        }

        public List<List<object>> executeQuery(string query, Dictionary<string, object> paramList = null) {
            if (openConnection()) {
                try {
                    List<List<object>> temp = new List<List<object>>();
                    MySqlCommand command = new MySqlCommand(query, conn);
                    if (paramList != null)
                        foreach (KeyValuePair<string, object> pair in paramList) {
                            command.Parameters.AddWithValue(pair.Key, pair.Value);
                        }
                    MySqlDataReader reader = command.ExecuteReader();
                    while (reader.Read()) {
                        List<object> li = new List<object>(reader.FieldCount);
                        for (int i = 0; i < reader.FieldCount; i++) {
                            li.Add(reader.GetValue(i));
                        }
                        temp.Add(li);
                    }
                    return temp;
                } catch (MySqlException e) {
                    lastError = e.Message;
                    lastErrorCode = e.Number;
                } finally {
                    closeConnection();
                }
            }
            return null;
        }

        public object executeScalar(string query, Dictionary<string, object> paramList = null) {
            if (openConnection()) {
                try {
                    MySqlCommand command = new MySqlCommand(query, conn);
                    if (paramList != null)
                        foreach (KeyValuePair<string, object> pair in paramList) {
                            command.Parameters.AddWithValue(pair.Key, pair.Value);
                        }
                    return command.ExecuteScalar();
                } catch (MySqlException e) {
                    lastError = e.Message;
                    lastErrorCode = e.Number;
                } finally {
                    closeConnection();
                }
            }
            return null;
        }

        public int executeNonQuery(string query, Dictionary<string, object> paramList = null) {
            if (openConnection()) {
                try {
                    MySqlCommand command = new MySqlCommand(query, conn);
                    if (paramList != null)
                        foreach (KeyValuePair<string, object> pair in paramList) {
                            command.Parameters.AddWithValue(pair.Key, pair.Value);
                        }
                    var t = command.ExecuteNonQuery();
                    lastInsertedId = command.LastInsertedId;
                    return t;
                } catch (MySqlException e) {
                    lastError = e.Message;
                    lastErrorCode = e.Number;
                } finally {
                    closeConnection();
                }
            }
            return -1;
        }

        public int getLastErrorCode() {
            return lastErrorCode;
        }

        public string getLastError() {
            return lastError;
        }

        public long getLastInsertedId() {
            return lastInsertedId;
        }
    }
}

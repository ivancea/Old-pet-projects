using System.Collections.Generic;
using System.Configuration;

namespace MtgDatabaseClient {
    class AppConfig {
        static Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);

        static public string getSetting(string setting) {
            return ConfigurationManager.AppSettings[setting] + "";
        }

        static public bool getSettingBool(string setting, bool defaultIfNull = false) {
            bool ret;
            if (!bool.TryParse(ConfigurationManager.AppSettings[setting], out ret)) {
                return defaultIfNull;
            }
            return ret;
        }

        static public int getSettingInt(string setting, int defaultIfNull = -1) {
            int ret;
            if (!int.TryParse(ConfigurationManager.AppSettings[setting], out ret)) {
                return defaultIfNull;
            }
            return ret;
        }

        static public double getSettingDouble(string setting, double defaultIfNull = -1) {
            double ret;
            if (!double.TryParse(ConfigurationManager.AppSettings[setting], out ret)) {
                return defaultIfNull;
            }
            return ret;
        }

        static public void setSetting(string setting, string value) {
            if (config.AppSettings.Settings[setting] != null) {
                config.AppSettings.Settings[setting].Value = value;
            } else {
                config.AppSettings.Settings.Add(setting, value);
            }
            config.Save();
            refresh();
        }

        static public void setSettings(Dictionary<string, string> settings) {
            foreach (KeyValuePair<string, string> setting in settings) {
                if (config.AppSettings.Settings[setting.Key] != null) {
                    config.AppSettings.Settings[setting.Key].Value = setting.Value;
                } else {
                    config.AppSettings.Settings.Add(setting.Key, setting.Value);
                }
            }
            config.Save();
            refresh();
        }

        static public void setSettings(Dictionary<string, object> settings) {
            foreach (KeyValuePair<string, object> setting in settings) {
                if (config.AppSettings.Settings[setting.Key] != null) {
                    config.AppSettings.Settings[setting.Key].Value = setting.Value.ToString();
                } else {
                    config.AppSettings.Settings.Add(setting.Key, setting.Value.ToString());
                }
            }
            config.Save();
            refresh();
        }

        static public void refresh() {
            ConfigurationManager.RefreshSection("appSettings");
            config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
        }
    }
}

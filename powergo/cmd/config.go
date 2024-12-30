/*
Copyright © 2024 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"
	"net/http"

	"github.com/hussein-mourad/esp8266-remote-pc-control/powergo/internal/config"
	"github.com/spf13/cobra"
)

var (
	wifi             = false
	printDefaultCfg  = false
	printCfgFilePath = false
)

// configCmd represents the config command
var configCmd = &cobra.Command{
	Use:   "config",
	Short: "Configure wifi",
	Long: "Enables access point mode to configure wifi\n" +
		"SSID: " + cfg.AP.SSID + "\n" +
		"Password: " + cfg.AP.Password + "\n",
	Run: func(cmd *cobra.Command, args []string) {
		if wifi {
			req.Send(http.MethodPost, cfg.API.URL+"/wifi/config")
		} else if printDefaultCfg {
			fmt.Println(config.GetDefaultConfigStr())
		} else if printCfgFilePath {
			fmt.Println(cfgFile)
		} else {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(configCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// configCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	configCmd.Flags().BoolVarP(&wifi, "wifi", "w", wifi, "Put ESP8266 in wifi config mode")
	configCmd.Flags().BoolVarP(&printDefaultCfg, "default", "d", wifi, "Prints default config")
	configCmd.Flags().BoolVarP(&printCfgFilePath, "file", "f", printCfgFilePath, "Print config file path")
}

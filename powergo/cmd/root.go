/*
Copyright © 2024 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"
	"os"

	"github.com/hussein-mourad/esp8266-remote-pc-control/powergo/internal/config"
	"github.com/hussein-mourad/esp8266-remote-pc-control/powergo/internal/request"
	"github.com/spf13/cobra"
)

var (
	defaultCfgFile = config.GetDefaultConfigFilePath()
	cfg            = config.LoadConfig(defaultCfgFile)
	cfgFile        = defaultCfgFile
	req            *request.Handler
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   config.AppName,
	Short: "Manage pc remotely using esp8266",
	// Uncomment the following line if your bare application
	// has an action associated with it:
	// Run: func(cmd *cobra.Command, args []string) {},
	PersistentPreRunE: func(cmd *cobra.Command, args []string) error {
		// Load the config once before running any command
		configFile, _ := cmd.Flags().GetString("config")
		req = request.NewHandler(cfg.Web.Username, cfg.Web.Password)
		var err error
		cfg = config.LoadConfig(configFile)
		if err != nil {
			return fmt.Errorf("Error loading config: %v", err)
		}
		return nil
	},
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {
	// Here you will define your flags and configuration settings.
	// Cobra supports persistent flags, which, if defined here,
	// will be global for your application.

	rootCmd.PersistentFlags().StringVarP(&cfgFile, "config", "c", defaultCfgFile, "config file")

	// Cobra also supports local flags, which will only run
	// when this action is called directly.
}

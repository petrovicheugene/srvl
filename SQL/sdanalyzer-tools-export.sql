-- Creator:       MySQL Workbench 6.3.7/ExportSQLite Plugin 0.1.0
-- Author:        Progger
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2016-09-04 15:44
-- Created:       2016-09-04 14:47
PRAGMA foreign_keys = OFF;

-- Schema: sdAnalyzer
ATTACH "sdAnalyzer.sdb" AS "sdAnalyzer";
BEGIN;
CREATE TABLE "sdAnalyzer"."chem_elements_compositions"(
  "name" VARCHAR(45) PRIMARY KEY NOT NULL
);
CREATE TABLE "sdAnalyzer"."calibration_stacks"(
  "name" VARCHAR(45) PRIMARY KEY NOT NULL,
  "chem_elements_compositions_name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name"),
  CONSTRAINT "fk_calibration_stacks_chem_elements_compositions1"
    FOREIGN KEY("chem_elements_compositions_name")
    REFERENCES "chem_elements_compositions"("name")
);
CREATE INDEX "sdAnalyzer"."calibration_stacks.fk_calibration_stacks_chem_elements_compositions1_idx" ON "calibration_stacks" ("chem_elements_compositions_name");
CREATE TABLE "sdAnalyzer"."measuring_conditions"(
  "gain_factor" INTEGER NOT NULL,
  "exposition" INTEGER NOT NULL,
  PRIMARY KEY("gain_factor","exposition")
);
CREATE TABLE "sdAnalyzer"."calibrations"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  "name" VARCHAR(45),
  "measuring_condition_id" INTEGER,
  "measuring_conditions_gain_factor" INTEGER NOT NULL,
  "measuring_conditions_exposition" INTEGER NOT NULL,
  CONSTRAINT "id_UNIQUE"
    UNIQUE("id"),
  CONSTRAINT "fk_calibrations_measuring_conditions"
    FOREIGN KEY("measuring_conditions_gain_factor","measuring_conditions_exposition")
    REFERENCES "measuring_conditions"("gain_factor","exposition")
);
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_measuring_conditions_idx" ON "calibrations" ("measuring_conditions_gain_factor","measuring_conditions_exposition");
CREATE TABLE "sdAnalyzer"."stacked_calibrations"(
  "calibration_stack_name" VARCHAR(45) NOT NULL,
  "calibrations_id" INTEGER NOT NULL,
  PRIMARY KEY("calibration_stack_name","calibrations_id"),
  CONSTRAINT "fk_stacked_calibrations_calibration_stacks1"
    FOREIGN KEY("calibration_stack_name")
    REFERENCES "calibration_stacks"("name"),
  CONSTRAINT "fk_stacked_calibrations_calibrations1"
    FOREIGN KEY("calibrations_id")
    REFERENCES "calibrations"("id")
);
CREATE INDEX "sdAnalyzer"."stacked_calibrations.fk_stacked_calibrations_calibration_stacks1_idx" ON "stacked_calibrations" ("calibration_stack_name");
CREATE INDEX "sdAnalyzer"."stacked_calibrations.fk_stacked_calibrations_calibrations1_idx" ON "stacked_calibrations" ("calibrations_id");
COMMIT;

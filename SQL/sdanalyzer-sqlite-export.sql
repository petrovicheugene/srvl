-- Creator:       MySQL Workbench 6.3.7/ExportSQLite Plugin 0.1.0
-- Author:        Progger
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2016-09-08 14:24
-- Created:       2016-09-04 14:47
PRAGMA foreign_keys = OFF;

-- Schema: sdAnalyzer
ATTACH "sdAnalyzer.sdb" AS "sdAnalyzer";
BEGIN;

DROP TABLE IF EXISTS "sdAnalyzer"."measuring_conditions";
DROP TABLE IF EXISTS "sdAnalyzer"."chemicals";
DROP TABLE IF EXISTS "sdAnalyzer"."measurement_units";
DROP TABLE IF EXISTS "sdAnalyzer"."calibrations";
DROP TABLE IF EXISTS "sdAnalyzer"."calibration_stacks";
DROP TABLE IF EXISTS "sdAnalyzer"."stacked_calibrations";

CREATE TABLE "sdAnalyzer"."measuring_conditions"(
  "gain_factor" INTEGER NOT NULL,
  "exposition" INTEGER NOT NULL,
  PRIMARY KEY("gain_factor","exposition")
);
CREATE TABLE "sdAnalyzer"."chemicals"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "sdAnalyzer"."measurement_units"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "sdAnalyzer"."calibrations"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "chemicals_id" INTEGER NOT NULL,
  "calibration_data" TEXT NOT NULL,
  "measuring_conditions_gain_factor" INTEGER NOT NULL,
  "measuring_conditions_exposition" INTEGER NOT NULL,
  "measurement_units_id" INTEGER NOT NULL,
  PRIMARY KEY("id","chemicals_id","measuring_conditions_gain_factor","measuring_conditions_exposition"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name"),
  CONSTRAINT "fk_calibrations_chemicals"
    FOREIGN KEY("chemicals_id")
    REFERENCES "chemicals"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_calibrations_measuring_conditions1"
    FOREIGN KEY("measuring_conditions_gain_factor","measuring_conditions_exposition")
    REFERENCES "measuring_conditions"("gain_factor","exposition")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_calibrations_measurement_units1"
    FOREIGN KEY("measurement_units_id")
    REFERENCES "measurement_units"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_chemicals_idx" ON "calibrations" ("chemicals_id");
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_measuring_conditions1_idx" ON "calibrations" ("measuring_conditions_gain_factor","measuring_conditions_exposition");
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_measurement_units1_idx" ON "calibrations" ("measurement_units_id");
CREATE TABLE "sdAnalyzer"."calibration_stacks"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45),
  "chemicals_id" INTEGER NOT NULL,
  PRIMARY KEY("id","chemicals_id"),
  CONSTRAINT "fk_calibration_stacks_chemicals1"
    FOREIGN KEY("chemicals_id")
    REFERENCES "chemicals"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."calibration_stacks.fk_calibration_stacks_chemicals1_idx" ON "calibration_stacks" ("chemicals_id");
CREATE TABLE "sdAnalyzer"."stacked_calibrations"(
  "calibration_stacks_id" INTEGER NOT NULL,
  "calibration_stacks_chemicals_id" INTEGER NOT NULL,
  "calibrations_id" INTEGER NOT NULL,
  "calibrations_chemicals_id" INTEGER NOT NULL,
  "calibrations_measuring_conditions_gain_factor" INTEGER NOT NULL,
  "calibrations_measuring_conditions_exposition" INTEGER NOT NULL,
  PRIMARY KEY("calibration_stacks_id","calibration_stacks_chemicals_id","calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition"),
  CONSTRAINT "fk_stacked_calibrations_calibration_stacks1"
    FOREIGN KEY("calibration_stacks_id","calibration_stacks_chemicals_id")
    REFERENCES "calibration_stacks"("id","chemicals_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_stacked_calibrations_calibrations1"
    FOREIGN KEY("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition")
    REFERENCES "calibrations"("id","chemicals_id","measuring_conditions_gain_factor","measuring_conditions_exposition")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."stacked_calibrations.fk_stacked_calibrations_calibrations1_idx" ON "stacked_calibrations" ("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition");
COMMIT;

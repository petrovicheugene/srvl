-- Creator:       MySQL Workbench 6.3.7/ExportSQLite Plugin 0.1.0
-- Author:        Progger
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2016-09-16 16:35
-- Created:       2016-09-04 14:47
PRAGMA foreign_keys = OFF;

-- Schema: sdAnalyzer
ATTACH "sdAnalyzer.sdb" AS "sdAnalyzer";
BEGIN;

DROP TABLE IF EXISTS "sdAnalyzer"."sample_tasks";
DROP TABLE IF EXISTS "sdAnalyzer"."measuring_conditions";
DROP TABLE IF EXISTS "sdAnalyzer"."chemicals";
DROP TABLE IF EXISTS "sdAnalyzer"."series_tasks";
DROP TABLE IF EXISTS "sdAnalyzer"."measurement_units";
DROP TABLE IF EXISTS "sdAnalyzer"."calibrations";
DROP TABLE IF EXISTS "sdAnalyzer"."operators";
DROP TABLE IF EXISTS "sdAnalyzer"."sample_tasks_has_series_tasks";
DROP TABLE IF EXISTS "sdAnalyzer"."series";
DROP TABLE IF EXISTS "sdAnalyzer"."calibration_stacks";
DROP TABLE IF EXISTS "sdAnalyzer"."calibration_stacks_has_sample_tasks";
DROP TABLE IF EXISTS "sdAnalyzer"."calibrations_has_calibration_stacks";
DROP TABLE IF EXISTS "sdAnalyzer"."samples";


CREATE TABLE "sdAnalyzer"."sample_tasks"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
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
CREATE TABLE "sdAnalyzer"."series_tasks"(
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
CREATE TABLE "sdAnalyzer"."operators"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(80) NOT NULL
);
CREATE TABLE "sdAnalyzer"."sample_tasks_has_series_tasks"(
  "series_tasks_id" INTEGER NOT NULL,
  "sample_tasks_id" INTEGER NOT NULL,
  PRIMARY KEY("series_tasks_id","sample_tasks_id"),
  CONSTRAINT "fk_sample_tasks_has_series_tasks_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_sample_tasks_has_series_tasks_series_tasks1"
    FOREIGN KEY("series_tasks_id")
    REFERENCES "series_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_series_tasks1_idx" ON "sample_tasks_has_series_tasks" ("series_tasks_id");
CREATE INDEX "sdAnalyzer"."sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_sample_tasks1_idx" ON "sample_tasks_has_series_tasks" ("sample_tasks_id");
CREATE TABLE "sdAnalyzer"."series"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "date" DATE NOT NULL,
  "time" TIME NOT NULL,
  "series_tasks_id" INTEGER NOT NULL,
  "operators_id" INTEGER NOT NULL,
  PRIMARY KEY("id","operators_id"),
  CONSTRAINT "fk_series_series_tasks1"
    FOREIGN KEY("series_tasks_id")
    REFERENCES "series_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_series_operators1"
    FOREIGN KEY("operators_id")
    REFERENCES "operators"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."series.fk_series_series_tasks1_idx" ON "series" ("series_tasks_id");
CREATE INDEX "sdAnalyzer"."series.fk_series_operators1_idx" ON "series" ("operators_id");
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
CREATE TABLE "sdAnalyzer"."calibration_stacks_has_sample_tasks"(
  "sample_tasks_id" INTEGER NOT NULL,
  "calibration_stacks_id" INTEGER NOT NULL,
  "calibration_stacks_chemicals_id" INTEGER NOT NULL,
  PRIMARY KEY("sample_tasks_id","calibration_stacks_id","calibration_stacks_chemicals_id"),
  CONSTRAINT "fk_calibration_stacks_has_sample_tasks_calibration_stacks1"
    FOREIGN KEY("calibration_stacks_id","calibration_stacks_chemicals_id")
    REFERENCES "calibration_stacks"("id","chemicals_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_calibration_stacks_has_sample_tasks_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."calibration_stacks_has_sample_tasks.fk_calibration_stacks_has_sample_tasks_sample_tasks1_idx" ON "calibration_stacks_has_sample_tasks" ("sample_tasks_id");
CREATE INDEX "sdAnalyzer"."calibration_stacks_has_sample_tasks.fk_calibration_stacks_has_sample_tasks_calibration_stacks1_idx" ON "calibration_stacks_has_sample_tasks" ("calibration_stacks_id","calibration_stacks_chemicals_id");
CREATE TABLE "sdAnalyzer"."calibrations_has_calibration_stacks"(
  "calibrations_id" INTEGER NOT NULL,
  "calibrations_chemicals_id" INTEGER NOT NULL,
  "calibrations_measuring_conditions_gain_factor" INTEGER NOT NULL,
  "calibrations_measuring_conditions_exposition" INTEGER NOT NULL,
  "calibration_stacks_id" INTEGER NOT NULL,
  "calibration_stacks_chemicals_id" INTEGER NOT NULL,
  PRIMARY KEY("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition","calibration_stacks_id","calibration_stacks_chemicals_id"),
  CONSTRAINT "fk_calibrations_has_calibration_stacks_calibrations1"
    FOREIGN KEY("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition")
    REFERENCES "calibrations"("id","chemicals_id","measuring_conditions_gain_factor","measuring_conditions_exposition")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_calibrations_has_calibration_stacks_calibration_stacks1"
    FOREIGN KEY("calibration_stacks_id","calibration_stacks_chemicals_id")
    REFERENCES "calibration_stacks"("id","chemicals_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibration_stacks1_idx" ON "calibrations_has_calibration_stacks" ("calibration_stacks_id","calibration_stacks_chemicals_id");
CREATE INDEX "sdAnalyzer"."calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibrations1_idx" ON "calibrations_has_calibration_stacks" ("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_gain_factor","calibrations_measuring_conditions_exposition");
CREATE TABLE "sdAnalyzer"."samples"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45),
  "series_id" INTEGER NOT NULL,
  PRIMARY KEY("id","series_id"),
  CONSTRAINT "fk_samples_series1"
    FOREIGN KEY("series_id")
    REFERENCES "series"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."samples.fk_samples_series1_idx" ON "samples" ("series_id");
COMMIT;

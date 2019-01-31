-- Creator:       MySQL Workbench 6.3.7/ExportSQLite Plugin 0.1.0
-- Author:        Progger
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2019-01-29 14:21
-- Created:       2016-09-04 14:47
PRAGMA foreign_keys = OFF;

-- Schema: sdAnalyzer
ATTACH "sdAnalyzer.sdb" AS "sdAnalyzer";
BEGIN;
CREATE TABLE "sdAnalyzer"."gain_factors"(
  "gain_factor" INTEGER PRIMARY KEY NOT NULL,
  "energyFactorK0" DOUBLE,
  "energyFactorK1" DOUBLE,
  "energyFactorK2" DOUBLE
);
CREATE TABLE "sdAnalyzer"."sample_tasks"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "sample_name_template" VARCHAR(45) NOT NULL,
  "description" TEXT,
  "date" DATE,
  "time" TIME,
  "actuality" BOOL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "sdAnalyzer"."measuring_conditions"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "gain_factor" INTEGER NOT NULL,
  "exposition" INTEGER NOT NULL,
  CONSTRAINT "fk_calibrations_measuring_conditions1"
    FOREIGN KEY("gain_factor")
    REFERENCES "gain_factors"("gain_factor")
    ON DELETE RESTRICT
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."measuring_conditions.fk_calibrations_measuring_conditions1_idx" ON "measuring_conditions" ("gain_factor");
CREATE TABLE "sdAnalyzer"."conditions_has_sample_tasks"(
  "id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
  "sample_tasks_id" INTEGER NOT NULL,
  PRIMARY KEY("id","measuring_conditions_id","sample_tasks_id"),
  CONSTRAINT "fk_measuring_conditions_has_sample_tasks_measuring_conditions"
    FOREIGN KEY("measuring_conditions_id")
    REFERENCES "measuring_conditions"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_measuring_conditions_has_sample_tasks_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."conditions_has_sample_tasks.fk_measuring_conditions_has_sample_tasks_sample_tasks1_idx" ON "conditions_has_sample_tasks" ("sample_tasks_id");
CREATE INDEX "sdAnalyzer"."conditions_has_sample_tasks.fk_measuring_conditions_has_sample_tasks_measuring_conditio_idx" ON "conditions_has_sample_tasks" ("measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."chemicals"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "sdAnalyzer"."series_tasks"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "description" TEXT,
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
  "description" TEXT,
  "calibration_data" BLOB NOT NULL,
  "chemicals_id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
  "measurement_units_id" INTEGER,
  "date" DATE,
  "time" TIME,
  "actuality" BOOL,
  PRIMARY KEY("id","chemicals_id","measuring_conditions_id"),
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name"),
  CONSTRAINT "fk_calibrations_chemicals1"
    FOREIGN KEY("chemicals_id")
    REFERENCES "chemicals"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_calibrations_measurement_units1"
    FOREIGN KEY("measurement_units_id")
    REFERENCES "measurement_units"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_calibrations_measuring_conditions1"
    FOREIGN KEY("measuring_conditions_id")
    REFERENCES "measuring_conditions"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_chemicals1_idx" ON "calibrations" ("chemicals_id");
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_measurement_units1_idx" ON "calibrations" ("measurement_units_id");
CREATE INDEX "sdAnalyzer"."calibrations.fk_calibrations_measuring_conditions1_idx" ON "calibrations" ("measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."operators"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(80) NOT NULL,
  "password" BLOB
);
CREATE TABLE "sdAnalyzer"."series"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "date" DATE NOT NULL,
  "time" TIME NOT NULL,
  "operators_id" INTEGER,
  "series_tasks_id" INTEGER,
  CONSTRAINT "fk_series_operators1"
    FOREIGN KEY("operators_id")
    REFERENCES "operators"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_series_series_tasks1"
    FOREIGN KEY("series_tasks_id")
    REFERENCES "series_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."series.fk_series_operators1_idx" ON "series" ("operators_id");
CREATE INDEX "sdAnalyzer"."series.fk_series_series_tasks1_idx" ON "series" ("series_tasks_id");
CREATE TABLE "sdAnalyzer"."calibration_stacks"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45),
  "description" TEXT,
  "chemicals_id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
  "date" DATE,
  "time" TIME,
  "actuality" BOOL,
  PRIMARY KEY("id","chemicals_id","measuring_conditions_id"),
  CONSTRAINT "fk_calibration_stacks_chemicals1"
    FOREIGN KEY("chemicals_id")
    REFERENCES "chemicals"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_calibration_stacks_measuring_conditions1"
    FOREIGN KEY("measuring_conditions_id")
    REFERENCES "measuring_conditions"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."calibration_stacks.fk_calibration_stacks_chemicals1_idx" ON "calibration_stacks" ("chemicals_id");
CREATE INDEX "sdAnalyzer"."calibration_stacks.fk_calibration_stacks_measuring_conditions1_idx" ON "calibration_stacks" ("measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."sample_tasks_has_series_tasks"(
  "id" INTEGER NOT NULL,
  "sample_tasks_id" INTEGER NOT NULL,
  "series_tasks_id" INTEGER NOT NULL,
  PRIMARY KEY("id","sample_tasks_id","series_tasks_id"),
  CONSTRAINT "fk_sample_tasks_has_series_tasks_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_sample_tasks_has_series_tasks_series_tasks1"
    FOREIGN KEY("series_tasks_id")
    REFERENCES "series_tasks"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_series_tasks1_idx" ON "sample_tasks_has_series_tasks" ("series_tasks_id");
CREATE INDEX "sdAnalyzer"."sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_sample_tasks1_idx" ON "sample_tasks_has_series_tasks" ("sample_tasks_id");
CREATE TABLE "sdAnalyzer"."calibration_stacks_has_conditions_has_sample_tasks"(
  "calibration_stacks_id" INTEGER NOT NULL,
  "calibration_stacks_chemicals_id" INTEGER NOT NULL,
  "calibration_stacks_measuring_conditions_id" INTEGER NOT NULL,
  "conditions_has_sample_tasks_id" INTEGER NOT NULL,
  "conditions_has_sample_tasks_measuring_conditions_id" INTEGER NOT NULL,
  "conditions_has_sample_tasks_sample_tasks_id" INTEGER NOT NULL,
  PRIMARY KEY("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id","conditions_has_sample_tasks_id","conditions_has_sample_tasks_measuring_conditions_id","conditions_has_sample_tasks_sample_tasks_id"),
  CONSTRAINT "fk_calibration_stacks_has_conditions_has_sample_tasks_calibra1"
    FOREIGN KEY("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id")
    REFERENCES "calibration_stacks"("id","chemicals_id","measuring_conditions_id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_calibration_stacks_has_conditions_has_sample_tasks_conditi1"
    FOREIGN KEY("conditions_has_sample_tasks_id","conditions_has_sample_tasks_measuring_conditions_id","conditions_has_sample_tasks_sample_tasks_id")
    REFERENCES "conditions_has_sample_tasks"("id","measuring_conditions_id","sample_tasks_id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."calibration_stacks_has_conditions_has_sample_tasks.fk_calibration_stacks_has_conditions_has_sample_tasks_condi_idx" ON "calibration_stacks_has_conditions_has_sample_tasks" ("conditions_has_sample_tasks_id","conditions_has_sample_tasks_measuring_conditions_id","conditions_has_sample_tasks_sample_tasks_id");
CREATE INDEX "sdAnalyzer"."calibration_stacks_has_conditions_has_sample_tasks.fk_calibration_stacks_has_conditions_has_sample_tasks_calib_idx" ON "calibration_stacks_has_conditions_has_sample_tasks" ("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."samples"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45),
  "series_id" INTEGER NOT NULL,
  "sample_tasks_id" INTEGER NOT NULL,
  CONSTRAINT "fk_samples_series1"
    FOREIGN KEY("series_id")
    REFERENCES "series"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_samples_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."samples.fk_samples_series1_idx" ON "samples" ("series_id");
CREATE INDEX "sdAnalyzer"."samples.fk_samples_sample_tasks1_idx" ON "samples" ("sample_tasks_id");
CREATE TABLE "sdAnalyzer"."measured_samples"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "series_id" INTEGER NOT NULL,
  "sample_tasks_id" INTEGER NOT NULL,
  CONSTRAINT "fk_measured_samples_series1"
    FOREIGN KEY("series_id")
    REFERENCES "series"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_measured_samples_sample_tasks1"
    FOREIGN KEY("sample_tasks_id")
    REFERENCES "sample_tasks"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."measured_samples.fk_measured_samples_series1_idx" ON "measured_samples" ("series_id");
CREATE INDEX "sdAnalyzer"."measured_samples.fk_measured_samples_sample_tasks1_idx" ON "measured_samples" ("sample_tasks_id");
CREATE TABLE "sdAnalyzer"."calibrations_has_calibration_stacks"(
  "calibrations_id" INTEGER NOT NULL,
  "calibrations_chemicals_id" INTEGER NOT NULL,
  "calibrations_measuring_conditions_id" INTEGER NOT NULL,
  "calibration_stacks_id" INTEGER NOT NULL,
  "calibration_stacks_chemicals_id" INTEGER NOT NULL,
  "calibration_stacks_measuring_conditions_id" INTEGER NOT NULL,
  "calibration_min_limit" DOUBLE,
  "calibration_max_limit" DOUBLE,
  PRIMARY KEY("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_id","calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id"),
  CONSTRAINT "fk_calibrations_has_calibration_stacks_calibrations1"
    FOREIGN KEY("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_id")
    REFERENCES "calibrations"("id","chemicals_id","measuring_conditions_id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "fk_calibrations_has_calibration_stacks_calibration_stacks1"
    FOREIGN KEY("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id")
    REFERENCES "calibration_stacks"("id","chemicals_id","measuring_conditions_id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "sdAnalyzer"."calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibration_stacks1_idx" ON "calibrations_has_calibration_stacks" ("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id");
CREATE INDEX "sdAnalyzer"."calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibrations1_idx" ON "calibrations_has_calibration_stacks" ("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."spectra"(
  "id" INTEGER NOT NULL,
  "spectrum" BLOB NOT NULL,
  "samples_id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
  PRIMARY KEY("id","samples_id","measuring_conditions_id"),
  CONSTRAINT "fk_spectra_samples1"
    FOREIGN KEY("samples_id")
    REFERENCES "samples"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_spectra_measuring_conditions1"
    FOREIGN KEY("measuring_conditions_id")
    REFERENCES "measuring_conditions"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."spectra.fk_spectra_samples1_idx" ON "spectra" ("samples_id");
CREATE INDEX "sdAnalyzer"."spectra.fk_spectra_measuring_conditions1_idx" ON "spectra" ("measuring_conditions_id");
CREATE TABLE "sdAnalyzer"."measured_chemicals"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "chemicals_id" INTEGER NOT NULL,
  "measured_spectra_id" INTEGER NOT NULL,
  CONSTRAINT "fk_measured_chemicals_chemicals1"
    FOREIGN KEY("chemicals_id")
    REFERENCES "chemicals"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_measured_chemicals_measured_spectra1"
    FOREIGN KEY("measured_spectra_id")
    REFERENCES "measured_spectra"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."measured_chemicals.fk_measured_chemicals_chemicals1_idx" ON "measured_chemicals" ("chemicals_id");
CREATE INDEX "sdAnalyzer"."measured_chemicals.fk_measured_chemicals_measured_spectra1_idx" ON "measured_chemicals" ("measured_spectra_id");
CREATE TABLE "sdAnalyzer"."measured_spectra"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "spectrum_data" BLOB NOT NULL,
  "measured_samples_id" INTEGER NOT NULL,
  CONSTRAINT "fk_measured_spectra_measured_samples1"
    FOREIGN KEY("measured_samples_id")
    REFERENCES "measured_samples"("id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "sdAnalyzer"."measured_spectra.fk_measured_spectra_measured_samples1_idx" ON "measured_spectra" ("measured_samples_id");
COMMIT;

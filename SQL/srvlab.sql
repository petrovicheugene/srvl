-- Creator:       MySQL Workbench 6.3.7/ExportSQLite Plugin 0.1.0
-- Author:        Progger
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2018-09-06 15:35
-- Created:       2016-09-04 14:47
PRAGMA foreign_keys = OFF;

-- Schema: srvlab
ATTACH "srvlab.sdb" AS "srvlab";
BEGIN;
CREATE TABLE "gain_factors"(
  "gain_factor" INTEGER PRIMARY KEY NOT NULL,
  "energyFactorK0" DOUBLE,
  "energyFactorK1" DOUBLE,
  "energyFactorK2" DOUBLE
);
CREATE TABLE "sample_tasks"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "sample_name_template" VARCHAR(45) NOT NULL,
  "description" TEXT,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "series"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "date" DATE NOT NULL,
  "time" TIME NOT NULL
);
CREATE TABLE "measuring_conditions"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "gain_factor" INTEGER NOT NULL,
  "exposition" INTEGER NOT NULL,
  CONSTRAINT "fk_calibrations_measuring_conditions1"
    FOREIGN KEY("gain_factor")
    REFERENCES "gain_factors"("gain_factor")
    ON DELETE RESTRICT
    ON UPDATE RESTRICT
);
CREATE INDEX "measuring_conditions.fk_calibrations_measuring_conditions1_idx" ON "measuring_conditions" ("gain_factor");
CREATE TABLE "conditions_has_sample_tasks"(
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
CREATE INDEX "conditions_has_sample_tasks.fk_measuring_conditions_has_sample_tasks_sample_tasks1_idx" ON "conditions_has_sample_tasks" ("sample_tasks_id");
CREATE INDEX "conditions_has_sample_tasks.fk_measuring_conditions_has_sample_tasks_measuring_conditio_idx" ON "conditions_has_sample_tasks" ("measuring_conditions_id");
CREATE TABLE "chemicals"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "series_tasks"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "description" TEXT,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "samples"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45)
);
CREATE TABLE "measurement_units"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  CONSTRAINT "name_UNIQUE"
    UNIQUE("name")
);
CREATE TABLE "calibrations"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45) NOT NULL,
  "description" TEXT,
  "calibration_data" BLOB NOT NULL,
  "chemicals_id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
  "measurement_units_id" INTEGER,
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
CREATE INDEX "calibrations.fk_calibrations_chemicals1_idx" ON "calibrations" ("chemicals_id");
CREATE INDEX "calibrations.fk_calibrations_measurement_units1_idx" ON "calibrations" ("measurement_units_id");
CREATE INDEX "calibrations.fk_calibrations_measuring_conditions1_idx" ON "calibrations" ("measuring_conditions_id");
CREATE TABLE "operators"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" VARCHAR(80) NOT NULL,
  "password" BLOB
);
CREATE TABLE "calibration_stacks"(
  "id" INTEGER NOT NULL,
  "name" VARCHAR(45),
  "description" TEXT,
  "chemicals_id" INTEGER NOT NULL,
  "measuring_conditions_id" INTEGER NOT NULL,
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
CREATE INDEX "calibration_stacks.fk_calibration_stacks_chemicals1_idx" ON "calibration_stacks" ("chemicals_id");
CREATE INDEX "calibration_stacks.fk_calibration_stacks_measuring_conditions1_idx" ON "calibration_stacks" ("measuring_conditions_id");
CREATE TABLE "sample_tasks_has_series_tasks"(
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
CREATE INDEX "sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_series_tasks1_idx" ON "sample_tasks_has_series_tasks" ("series_tasks_id");
CREATE INDEX "sample_tasks_has_series_tasks.fk_sample_tasks_has_series_tasks_sample_tasks1_idx" ON "sample_tasks_has_series_tasks" ("sample_tasks_id");
CREATE TABLE "calibration_stacks_has_conditions_has_sample_tasks"(
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
CREATE INDEX "calibration_stacks_has_conditions_has_sample_tasks.fk_calibration_stacks_has_conditions_has_sample_tasks_condi_idx" ON "calibration_stacks_has_conditions_has_sample_tasks" ("conditions_has_sample_tasks_id","conditions_has_sample_tasks_measuring_conditions_id","conditions_has_sample_tasks_sample_tasks_id");
CREATE INDEX "calibration_stacks_has_conditions_has_sample_tasks.fk_calibration_stacks_has_conditions_has_sample_tasks_calib_idx" ON "calibration_stacks_has_conditions_has_sample_tasks" ("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id");
CREATE TABLE "calibrations_has_calibration_stacks"(
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
CREATE INDEX "calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibration_stacks1_idx" ON "calibrations_has_calibration_stacks" ("calibration_stacks_id","calibration_stacks_chemicals_id","calibration_stacks_measuring_conditions_id");
CREATE INDEX "calibrations_has_calibration_stacks.fk_calibrations_has_calibration_stacks_calibrations1_idx" ON "calibrations_has_calibration_stacks" ("calibrations_id","calibrations_chemicals_id","calibrations_measuring_conditions_id");
COMMIT;

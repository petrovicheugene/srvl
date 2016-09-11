-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema sdAnalyzer
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `sdAnalyzer` ;

-- -----------------------------------------------------
-- Schema sdAnalyzer
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `sdAnalyzer` DEFAULT CHARACTER SET utf8 ;
USE `sdAnalyzer` ;

-- -----------------------------------------------------
-- Table `chemicals`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `chemicals` ;

CREATE TABLE IF NOT EXISTS `chemicals` (
  `id` INT NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `measuring_conditions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `measuring_conditions` ;

CREATE TABLE IF NOT EXISTS `measuring_conditions` (
  `gain_factor` INT NOT NULL,
  `exposition` INT NOT NULL,
  PRIMARY KEY (`gain_factor`, `exposition`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `measurement_units`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `measurement_units` ;

CREATE TABLE IF NOT EXISTS `measurement_units` (
  `id` INT NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `calibrations`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `calibrations` ;

CREATE TABLE IF NOT EXISTS `calibrations` (
  `id` INT NOT NULL,
  `name` VARCHAR(45) NOT NULL COMMENT 'Calibration name - defined by user',
  `chemicals_id` INT NOT NULL,
  `calibration_data` TEXT NOT NULL COMMENT 'Calibration data - text in xml format as in calibration source file.',
  `measuring_conditions_gain_factor` INT NOT NULL,
  `measuring_conditions_exposition` INT NOT NULL,
  `measurement_units_id` INT NOT NULL,
  PRIMARY KEY (`id`, `chemicals_id`, `measuring_conditions_gain_factor`, `measuring_conditions_exposition`),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC),
  INDEX `fk_calibrations_chemicals_idx` (`chemicals_id` ASC),
  INDEX `fk_calibrations_measuring_conditions1_idx` (`measuring_conditions_gain_factor` ASC, `measuring_conditions_exposition` ASC),
  INDEX `fk_calibrations_measurement_units1_idx` (`measurement_units_id` ASC),
  CONSTRAINT `fk_calibrations_chemicals`
    FOREIGN KEY (`chemicals_id`)
    REFERENCES `chemicals` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_calibrations_measuring_conditions1`
    FOREIGN KEY (`measuring_conditions_gain_factor` , `measuring_conditions_exposition`)
    REFERENCES `measuring_conditions` (`gain_factor` , `exposition`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_calibrations_measurement_units1`
    FOREIGN KEY (`measurement_units_id`)
    REFERENCES `measurement_units` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

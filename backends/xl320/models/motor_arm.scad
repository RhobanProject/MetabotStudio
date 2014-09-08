include <motor.scad>;
use <arm.scad>;

MotorMark = false;

module motorArm() {
  if (MotorMark) {
	  marker("motor");
  } else {
	  motor();
	  translate([0,0,MotorDepth/2])
	    arm(true);
	  translate([0,0,-MotorDepth/2-OlloWidth])
	    arm();
  } 
}

motorArm();

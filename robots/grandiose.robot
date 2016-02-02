{
   "backend" : "xl-320",
   "parameters" : {
      "nrlegs" : "5"
   },
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {},
               "component" : "bottom_leg_angle",
               "parameters" : {
                  "FixationAngle" : "0",
                  "SizeA" : "60",
                  "SizeB" : "20",
                  "SizeBottom" : "10",
                  "SizeC" : "20",
                  "SizeTop" : "15",
                  "Thickness" : "2.2"
               }
            },
            "maximum" : 3.141592741012573,
            "minimum" : -3.141592741012573,
            "orientationX" : 0.0,
            "orientationY" : 0.0,
            "orientationZ" : 0.0,
            "remote" : 0,
            "zero" : 0.0
         },
         "1" : {
            "component" : {
               "anchors" : {},
               "component" : "arm_leg",
               "parameters" : {
                  "SizeA" : "60",
                  "SizeB" : "20",
                  "SizeBottom" : "10",
                  "SizeC" : "20",
                  "SizeTop" : "15",
                  "Thickness" : "2.2"
               }
            },
            "maximum" : 3.141592741012573,
            "minimum" : -3.141592741012573,
            "orientationX" : 0.0,
            "orientationY" : 0.0,
            "orientationZ" : 0.0,
            "remote" : 0,
            "zero" : 0.0
         }
      },
      "component" : "arm_to_side",
      "parameters" : {
         "Distance" : "45",
         "FirstPart" : "17",
         "Thickness" : "2.2"
      }
   }
}

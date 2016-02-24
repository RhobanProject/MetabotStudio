{
   "backend" : "xl-320",
   "parameters" : {
      "L" : {
         "max" : "200",
         "min" : "80",
         "val" : "80"
      },
      "L1" : {
         "max" : "200",
         "min" : "30",
         "val" : "45"
      },
      "L2" : {
         "max" : "200",
         "min" : "60",
         "val" : "65"
      },
      "L3" : {
         "max" : "200",
         "min" : "30",
         "val" : "30"
      },
      "W" : {
         "max" : "100",
         "min" : "40",
         "val" : "40"
      }
   },
   "tips" : 4,
   "tree" : {
      "anchors" : {
         "0" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg",
                                 "parameters" : {
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : -1.5707963705062866,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
            "zero" : 0
         },
         "1" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg",
                                 "parameters" : {
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : -1.5707963705062866,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
            "zero" : 0
         },
         "2" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg",
                                 "parameters" : {
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : -1.5707963705062866,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
            "zero" : 0
         },
         "3" : {
            "component" : {
               "anchors" : {
                  "0" : {
                     "component" : {
                        "anchors" : {
                           "0" : {
                              "component" : {
                                 "anchors" : {},
                                 "component" : "u_leg",
                                 "parameters" : {
                                    "Height" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "$L3"
                                    },
                                    "Thickness" : {
                                       "max" : "",
                                       "min" : "",
                                       "val" : "2.2"
                                    }
                                 }
                              },
                              "maximum" : 0,
                              "minimum" : -3.1415927410125732,
                              "orientationX" : 0,
                              "orientationY" : 0,
                              "orientationZ" : 0,
                              "remote" : 0,
                              "zero" : 0
                           }
                        },
                        "component" : "side_to_side",
                        "parameters" : {
                           "HolesToBorder" : {
                              "max" : "",
                              "min" : "",
                              "val" : "5"
                           },
                           "Size" : {
                              "max" : "",
                              "min" : "",
                              "val" : "$L2"
                           },
                           "Thickness" : {
                              "max" : "",
                              "min" : "",
                              "val" : "2.2"
                           }
                        }
                     },
                     "maximum" : 3.1415927410125732,
                     "minimum" : -3.1415927410125732,
                     "orientationX" : 0,
                     "orientationY" : 0,
                     "orientationZ" : -1.5707963705062866,
                     "remote" : 1,
                     "zero" : 0
                  }
               },
               "component" : "double_u",
               "parameters" : {
                  "Angle" : {
                     "max" : "",
                     "min" : "",
                     "val" : "90"
                  },
                  "Height" : {
                     "max" : "",
                     "min" : "",
                     "val" : "$L1"
                  },
                  "Radius" : {
                     "max" : "",
                     "min" : "",
                     "val" : "8"
                  },
                  "Screws" : {
                     "max" : "",
                     "min" : "",
                     "val" : "true"
                  },
                  "ScrewsCount" : {
                     "max" : "",
                     "min" : "",
                     "val" : "4"
                  },
                  "ScrewsDiameter" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.6"
                  },
                  "ScrewsSpacing" : {
                     "max" : "",
                     "min" : "",
                     "val" : "14"
                  },
                  "Thickness" : {
                     "max" : "",
                     "min" : "",
                     "val" : "2.2"
                  }
               }
            },
            "maximum" : 3.1415927410125732,
            "minimum" : -3.1415927410125732,
            "orientationX" : 0,
            "orientationY" : 0,
            "orientationZ" : 0,
            "remote" : 1,
            "zero" : 0
         }
      },
      "component" : "square_body",
      "parameters" : {
         "Legs" : {
            "max" : "",
            "min" : "",
            "val" : "4"
         },
         "LegsSpacing" : {
            "max" : "",
            "min" : "",
            "val" : "$L"
         },
         "Thickness" : {
            "max" : "",
            "min" : "",
            "val" : "2.2"
         },
         "Width" : {
            "max" : "",
            "min" : "",
            "val" : "$W"
         }
      }
   }
}

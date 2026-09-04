# Results summary

This folder contains lightweight result artifacts extracted from the project report and organized for GitHub documentation.

## Included artifacts

- `motor2_dc35_dataset.png`: measured motor-2 data under a constant 35% duty-cycle experiment.
- `arx_validation_dc35.png`: ARX validation on the constant-duty-cycle dataset.
- `arx_validation_random_dc.png`: ARX validation on the random-duty-cycle dataset.
- `grey_box_dynamic_model_function.png`: MATLAB function describing the investigated single-link grey-box model.
- `grey_box_estimation_script.png`: MATLAB grey-box estimation setup.
- `grey_box_parameter_setup.png`: parameter vector and estimation constraints used in grey-box experiments.
- `simulink_pi_tuning_loop.png`: Simulink loop used for preliminary PI tuning on an identified model.
- `hardware_issues_summary.csv`: structured summary of the main hardware limitations.
- `identification_summary.csv`: structured summary of the identification experiments.

## Interpretation

The results should be interpreted as part of an experimental reverse-engineering workflow. The firmware and modeling pipeline were implemented, but the final robot-level control validation was limited by hardware issues, especially encoder accessibility, electrical disturbances, and damaged encoder channels.
